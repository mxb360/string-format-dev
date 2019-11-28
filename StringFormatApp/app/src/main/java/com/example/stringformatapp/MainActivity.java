
package com.example.stringformatapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {
    static final String TAG = "string-format";
    static final String cmdString[] = { "保持不变", "转为小写", "转为小写", "大小写反转", "转为标题", "逆序"};

    EditText inputEditText;
    TextView outputTextView;

    Button keepButton;
    Button toUpperButton;
    Button toLowerButton;
    Button toggleButton;
    Button toTitleButton;
    Button reverseButton;
    Button clearButton;

    CheckBox syncOutputAndInputCheckBox;

    StringFormat stringFormat;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setUI();

        if (!openStringFormat()) {
            Log.e(TAG, "error: device open failed!");
            Toast.makeText(this, "device open failed!", Toast.LENGTH_SHORT).show();

        } else {
            Log.d(TAG, "device open successful!");
            Toast.makeText(this, "device open successful!", Toast.LENGTH_SHORT).show();
        }
    }

    private void setUI() {
        inputEditText = findViewById(R.id.editText17);
        outputTextView = findViewById(R.id.textView19);

        keepButton = findViewById(R.id.button39);
        toUpperButton = findViewById(R.id.button36);
        toLowerButton = findViewById(R.id.button37);
        toggleButton = findViewById(R.id.button38);
        toTitleButton = findViewById(R.id.button40);
        reverseButton = findViewById(R.id.button41);
        clearButton = findViewById(R.id.button42);

        syncOutputAndInputCheckBox = findViewById(R.id.checkBox);

        keepButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_KEEP);
            }
        });

        toUpperButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_TO_UPPER);
            }
        });

        toLowerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_TO_LOWER);
            }
        });

        toggleButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_TOGGLE);
            }
        });

        toTitleButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_TO_TITLE);

            }
        });

        reverseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCmd(StringFormat.SF_REVERSE);
            }
        });

        clearButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                inputEditText.setText("");
                outputTextView.setText("");
            }
        });
    }

    private boolean openStringFormat() {
        stringFormat = new StringFormat();
        return stringFormat.isOpenSuccessful();
    }

    private void sendCmd(int cmd) {

        if (!openStringFormat()) {
            Log.e(TAG, "error: device open failed!");
            Toast.makeText(this, "device open failed!", Toast.LENGTH_SHORT).show();
            return;
        }

        String inputString = inputEditText.getText().toString().trim();
        Log.d(TAG, "cmd: " + cmdString[cmd]);
        Log.d(TAG, "input" + ": " + inputString);

        if (inputString.equals("")) {
            Log.e(TAG, "error: " + "no input string");
            Toast.makeText(this, "no input string", Toast.LENGTH_SHORT).show();
            return;
        }

        String outputString = stringFormat.cmd(inputString, cmd);
        if (outputString == null) {
            Log.e(TAG, "error: " + "string format failed!");
            Toast.makeText(this, "string format failed!", Toast.LENGTH_SHORT).show();
            outputTextView.setText("");
        } else {
            outputTextView.setText(outputString);
            if (syncOutputAndInputCheckBox.isChecked())
                inputEditText.setText(outputString);
            Log.d(TAG, "output" + ": " + outputString);
        }
    }
}
