package com.example.stringformatapp;

public class StringFormat {
    private int sf;

    public static final int SF_KEEP = 0;
    public static final int SF_TO_UPPER = 1;
    public static final int SF_TO_LOWER = 2;
    public static final int SF_TOGGLE = 3;
    public static final int SF_TO_TITLE = 4;
    public static final int SF_REVERSE = 5;

    public StringFormat() {
        sf = open();
    }

    public boolean isOpenSuccessful() {
        return sf > 0;
    }

    public String cmd(String string, int cmd) {
        if (sf <= 0)
            return null;
        return cmd(string, cmd, sf);
    }

    public void colse() {
        if (sf > 0)
            close(sf);
    }


    private native int open();

    private native String cmd(String string, int cmd, int sf);

    private native int close(int sf);
}
