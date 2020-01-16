package com.ben.android.libqrencode_android;

import androidx.appcompat.app.AppCompatActivity;

import android.animation.ObjectAnimator;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.ben.android.qrencode.library.QREncode;
import com.ben.android.qrencode.library.QREncodeUtilities;

public class MainActivity extends AppCompatActivity {
    private CheckBox transparentBox, insertLogoBox,useOverlayBox;
    private ObjectAnimator transparentAnimator,overlayAnimator;
    private View backgroundView;
    private LinearLayout overlayList;
    private Bitmap selectOverlay;
    private HorizontalScrollView overlayContainer;
    private View.OnClickListener overlayClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            String tag =  v.getTag().toString();
            int id = getResources().getIdentifier("overlay_"+tag, "drawable", getPackageName());
            selectOverlay = BitmapFactory.decodeResource(getResources(), id);
            generate();
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        backgroundView = findViewById(R.id.background);
        transparentBox = findViewById(R.id.id_transparent);
        overlayContainer = findViewById(R.id.id_overlay_container);
        useOverlayBox = findViewById(R.id.id_use_overlay);
        useOverlayBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (overlayAnimator != null) {
                    overlayAnimator.cancel();
                }
                overlayContainer.setAlpha(isChecked ? 0f : 1.0f);
                overlayContainer.setVisibility(isChecked ? View.VISIBLE : View.GONE);
                overlayAnimator = ObjectAnimator.ofFloat(overlayContainer, "alpha", isChecked ? 0f : 1.0f, isChecked ? 1.0f : 0f);
                overlayAnimator.setDuration(500);
                overlayAnimator.start();

            }
        });

        insertLogoBox = findViewById(R.id.id_insert_logo);
        insertLogoBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                generate();
            }
        });
        transparentBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (transparentAnimator != null) {
                    transparentAnimator.cancel();
                }
                backgroundView.setAlpha(isChecked ? 0f : 1.0f);
                backgroundView.setVisibility(View.VISIBLE);
                transparentAnimator = ObjectAnimator.ofFloat(backgroundView, "alpha", isChecked ? 0f : 1.0f, isChecked ? 1.0f : 0f);
                transparentAnimator.setDuration(500);
                transparentAnimator.start();

                generate();
            }
        });
        initOverlay();
        generate();
    }
    public void initOverlay() {
        overlayList = findViewById(R.id.id_overlay_list);
        for (int i = 0; i < overlayList.getChildCount(); i++) {
            overlayList.getChildAt(i).setOnClickListener(overlayClickListener);
        }
    }

    public void generate(View view) {
        generate();
    }

    private void generate() {
        String qrsource = getEditTextContent(R.id.id_qrsource_tv);
        String sizes = getEditTextContent(R.id.id_qr_size);
        int qrsize = Integer.parseInt(TextUtils.isEmpty(sizes) ? "600" : sizes);

        TextView timeView = findViewById(R.id.id_generat_time);
        ImageView qrView = findViewById(R.id.qrview);

        long start = System.currentTimeMillis();
        //background color.
        int bgcolor = Color.WHITE;
        if (transparentBox.isChecked()) {
            bgcolor = Color.TRANSPARENT;

        }

        Bitmap bitmap;
        if (useOverlayBox.isChecked()) {
            bitmap = QREncode.encodeOverlay(qrsource, qrsize, bgcolor,
                    QREncodeUtilities.createQROverlay(selectOverlay, qrsize),
                    insertLogoBox.isChecked() ? getLogo(qrsize, 11) : null);
        }else{
            int color = Color.argb(255, getColorVal(R.id.id_qr_color_r), getColorVal(R.id.id_qr_color_g), getColorVal(R.id.id_qr_color_B));
            bitmap = QREncode.encode(qrsource, qrsize, bgcolor, color, insertLogoBox.isChecked() ? getLogo(qrsize, 11) : null);
        }

        long end = System.currentTimeMillis();
        if (bitmap != null) {
            qrView.setImageBitmap(bitmap);
            timeView.setText("Generation time:" + (end - start) + "ms");
        } else {
            Toast.makeText(this, "encode failed", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * get logo bitmao.
     *
     * @param qrsize
     * @param scale  scale baseline.
     * @return
     */
    private Bitmap getLogo(int qrsize, int scale) {
        BitmapFactory.Options opts = new BitmapFactory.Options();
        opts.inJustDecodeBounds = true;
        BitmapFactory.decodeResource(getResources(), R.drawable.icon_wechat, opts);
        int logoWidth = opts.outWidth;
        if (logoWidth >= qrsize) {
            Toast.makeText(this, "Logo width must be smaller than QR code", Toast.LENGTH_SHORT).show();
            return null;
        }
        opts.inJustDecodeBounds = false;
        opts.inSampleSize = scale / (qrsize / logoWidth);
        return BitmapFactory.decodeResource(getResources(), R.drawable.icon_wechat, opts);
    }

    private String getEditTextContent(int id) {
        return ((EditText) findViewById(id)).getText().toString();
    }

    private int getColorVal(int id) {
        String rs = getEditTextContent(id);
        return Integer.parseInt(TextUtils.isEmpty(rs) ? "0" : rs);
    }

}
