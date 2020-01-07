package com.ben.android.libqrencode_android;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.animation.ObjectAnimator;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Matrix;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.ben.android.qrencode.library.QREncode;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private CheckBox transparentBox;
    private ObjectAnimator animator;
    private View backgroundView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        backgroundView = findViewById(R.id.background);
        transparentBox = findViewById(R.id.id_transparent);
        transparentBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (animator != null) {
                    animator.cancel();
                }
                backgroundView.setAlpha(isChecked?0f:1.0f);
                backgroundView.setVisibility(View.VISIBLE);
                animator = ObjectAnimator.ofFloat(backgroundView, "alpha", isChecked ? 0f : 1.0f, isChecked ? 1.0f : 0f);
                animator.setDuration(500);
                animator.start();

                generate();
            }
        });
        generate();
    }

    public void generate(View view) {
        generate();
    }

    private void generate() {
        String qrsource = getEditTextContent(R.id.id_qrsource_tv);
        String sizes = getEditTextContent(R.id.id_qr_size);
        int qrsize = Integer.parseInt(TextUtils.isEmpty(sizes) ? "400" : sizes);

        TextView timeView = findViewById(R.id.id_generat_time);
        ImageView qrView = findViewById(R.id.qrview);

        long start = System.currentTimeMillis();
        //background color.
        int bgcolor = Color.WHITE;
        if (transparentBox.isChecked()) {
            bgcolor = Color.TRANSPARENT;

        }
        //qr color.
        int color = Color.argb(255, getColorVal(R.id.id_qr_color_r), getColorVal(R.id.id_qr_color_g), getColorVal(R.id.id_qr_color_B));
        Bitmap bitmap = QREncode.encode(qrsource, qrsize, bgcolor, color);
        long end = System.currentTimeMillis();
        if (bitmap != null) {
            qrView.setImageBitmap(bitmap);
            timeView.setText("Generation time:" + (end - start) + "ms");
        } else {
            Toast.makeText(this, "encode failed", Toast.LENGTH_SHORT).show();
        }
    }

    private String getEditTextContent(int id) {
        return ((EditText) findViewById(id)).getText().toString();
    }

    private int getColorVal(int id) {
        String rs = getEditTextContent(id);
        return Integer.parseInt(TextUtils.isEmpty(rs) ? "0" : rs);
    }

}
