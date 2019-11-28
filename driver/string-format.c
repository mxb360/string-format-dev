#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ctype.h>

#define CDEV_NAME           "string-format"
#define MAX_STRING_SIZE     1024

static void string_keep(char *dest, const char *src);
static void string_to_upper(char *dest, const char *src);
static void string_to_lower(char *dest, const char *src);
static void string_toggle(char *dest, const char *src);
static void string_reverse(char *dest, const char *src);
static void string_to_title(char *dest, const char *src);

static int string_format_open(struct inode *inode, struct file *filep);
static ssize_t string_format_read(struct file *filep, char __user *buf, size_t count, loff_t *offp);
static ssize_t string_format_write(struct file *filep, const char __user *buf, size_t count, loff_t *offp);
static long string_format_unlocked_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
static int string_format_release(struct inode *inode, struct file *filep);

static struct string_format_cdev_t {
    dev_t id;
    int major;
    struct cdev cdev;
    struct file_operations ops;
    struct class *class;
} string_format_cdev = {
    .ops = {
        .owner = THIS_MODULE,
        .open = string_format_open,
        .read = string_format_read,
        .write = string_format_write,
        .unlocked_ioctl = string_format_unlocked_ioctl,
        .release = string_format_release,
    },
};

static void (*string_format_func[])(char *, const char *) = {
    string_keep,
    string_to_upper,
    string_to_lower,
    string_toggle,
    string_to_title,
    string_reverse,
};

struct string_format_info_t {
    char old_string[MAX_STRING_SIZE + 1];
    char new_string[MAX_STRING_SIZE + 1];
    int flags;
};

static void string_keep(char *dest, const char *src)
{
    while ((*dest++ = *src++));
}


static void string_to_upper(char *dest, const char *src)
{
    while ((*dest++ = toupper(*src++)));
}

static void string_to_lower(char *dest, const char *src)
{
    while ((*dest++ = tolower(*src++)));
}

static void string_toggle(char *dest, const char *src)
{
    while ((*dest++ = isupper(*src) ? tolower(*src++) : toupper(*src++)));
}

static void string_to_title(char *dest, const char *src)
{
    int last = ' ';

    while (*src) {
        *dest++ = !isspace(*src) && isspace(last) ? toupper(*src) : tolower(*src);
        last = *src++;
    }
    *dest = 0;
}

static void string_reverse(char *dest, const char *src)
{
    int n = 0;
    while (*src++)  n++;
    src--;
    while (n--)     *dest++ = *--src;
    *dest = 0;
}


static int string_format_open(struct inode *inode, struct file *filep)
{
    struct string_format_info_t  *string_format_info;

    printk(CDEV_NAME ": open(pathname=%s, flags=%d)\n", CDEV_NAME, filep->f_flags);

    if ((string_format_info = (struct string_format_info_t *)kzalloc(sizeof (struct string_format_info_t), GFP_KERNEL)) == NULL) {
        printk(CDEV_NAME ": error: kzalloc failed!\n");
        return -1;
    }

    filep->private_data = string_format_info;    
    return 0;
}

static ssize_t string_format_read(struct file *filep, char __user *buf, size_t count, loff_t *offp)
{
    int str_len;
    int res_len;
    struct string_format_info_t *string_format_info = filep->private_data;

    string_format_func[string_format_info->flags](string_format_info->new_string, string_format_info->old_string);
    
    str_len = strlen(string_format_info->new_string);
    str_len = str_len < count - 1 ? str_len : count - 1;
    
    if ((res_len = copy_to_user(buf, string_format_info->new_string, str_len + 1))) {
        printk(CDEV_NAME ": error: copy_to_user failed: all: %d, faild: %d\n", str_len + 1, res_len);
        return 0;
    }

    printk(CDEV_NAME ": read(buf=\"%s\", count=%zd)\n", string_format_info->new_string, count);
    return str_len; 
}


static ssize_t string_format_write(struct file *filep, const char __user *buf, size_t count, loff_t *offp)
{
    int str_len;
    int res_len;
    struct string_format_info_t *string_format_info = (struct string_format_info_t *)filep->private_data;

    count = count < MAX_STRING_SIZE ? count : MAX_STRING_SIZE;

    if ((res_len = copy_from_user(string_format_info->old_string, buf, count))) {
        printk(CDEV_NAME ": error: copy_from_user failed: all: %zd, faild: %d\n", count, res_len);
        return 0;
    }

    string_format_info->old_string[count] = 0;
    printk(CDEV_NAME ": write(buf=\"%s\", count=%zd)\n", string_format_info->old_string, count);

    return str_len;
}

static long string_format_unlocked_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int max_cmd = sizeof string_format_func / sizeof string_format_func[0] - 1;
    struct string_format_info_t *string_format_info = (struct string_format_info_t *)filep->private_data;

    cmd = _IOC_NR(cmd);
    printk(CDEV_NAME ": ioctl(cmd=%u, arg=%lu)\n", cmd, arg);

    if (cmd > max_cmd) {
        printk(CDEV_NAME ": error: ioctl: invalid cmd: %d, cmd should be in [0, %d]\n", cmd, max_cmd);
        return -1;
    }

    string_format_info->flags = cmd;
    return 0; 
}

static int string_format_release(struct inode *inode, struct file *filep)
{
    struct string_format_info_t *string_format_info = (struct string_format_info_t *)filep->private_data;

    printk(CDEV_NAME ": close()\n");
    kfree(string_format_info);
    return 0;
}

static int __init string_format_init(void)
{
    printk(CDEV_NAME ": cdev init.\n");
    
    alloc_chrdev_region(&string_format_cdev.id, 0, 1, CDEV_NAME);
    string_format_cdev.major = MAJOR(string_format_cdev.id);
    
    cdev_init(&string_format_cdev.cdev, &string_format_cdev.ops);
    cdev_add(&string_format_cdev.cdev, string_format_cdev.id, 1);
    
    string_format_cdev.class = class_create(THIS_MODULE, CDEV_NAME);
    device_create(string_format_cdev.class, NULL, string_format_cdev.id, NULL, CDEV_NAME);
    
    return 0;
}

static void __exit string_format_exit(void)
{
    printk(CDEV_NAME ": cdev exit.\n");

    device_destroy(string_format_cdev.class, MKDEV(string_format_cdev.major, 0));
    class_destroy(string_format_cdev.class);
    cdev_del(&string_format_cdev.cdev);

    unregister_chrdev_region(string_format_cdev.id, 1);
}

module_init(string_format_init);
module_exit(string_format_exit);
MODULE_LICENSE("GPL");
