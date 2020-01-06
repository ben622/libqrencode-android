package com.ben.android.libqrencode_android;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.ben.android.qrencode.library.QREncode;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        generate();
    }

    public void generate(View view) {
        generate();
    }

    private void generate() {
        if (Build.VERSION.SDK_INT >= 23 ) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 4);
                return;
            }
        }

        String qrsource = getEditTextContent(R.id.id_qrsource_tv);
        String sizes = getEditTextContent(R.id.id_qr_size);
        int qrsize = Integer.parseInt(TextUtils.isEmpty(sizes) ? "200" : sizes);

        TextView timeView = findViewById(R.id.id_generat_time);
        ImageView qrView = findViewById(R.id.qrview);

        long start = System.currentTimeMillis();
        String path = getExternalCacheDir() + File.separator + start + ".bmp";
        int code = QREncode.encode(qrsource, qrsize, path, getColorVal(R.id.id_qr_color_r), getColorVal(R.id.id_qr_color_g), getColorVal(R.id.id_qr_color_B));
        long end = System.currentTimeMillis();
        if (code >= 0) {
            qrView.setImageBitmap(BitmapFactory.decodeFile(path));
            timeView.setText("Generation time:" + (end - start) + "ms，ouput:" + path);
        }else{
            Toast.makeText(this, "encode failed", Toast.LENGTH_SHORT).show();
        }
    }

    private String getEditTextContent(int id){
        return ((EditText) findViewById(id)).getText().toString();
    }

    private int getColorVal(int id) {
        String rs = getEditTextContent(id);
        return Integer.parseInt(TextUtils.isEmpty(rs) ? "0" : rs);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
           generate();
        }
    }
}
