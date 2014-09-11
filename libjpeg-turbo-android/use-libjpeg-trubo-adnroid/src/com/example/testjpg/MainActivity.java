package com.example.testjpg;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import com.pi.common.util.NativeUtil;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.os.Bundle;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		testJpeg();
	}

	private void testJpeg() {
		new Thread(new Runnable() {
			public void run() {
				try {
					int quality = 90;
					InputStream in = getResources().getAssets()
							.open("test1.jpg");
					Bitmap bit = BitmapFactory.decodeStream(in);
					File dirFile = getCacheDir();
					File originalFile = new File(dirFile, "original.jpg");
					FileOutputStream fileOutputStream = new FileOutputStream(
							originalFile);
					bit.compress(CompressFormat.JPEG, quality, fileOutputStream);
					File jpegTrueFile = new File(dirFile, "jpegtrue.jpg");
					File jpegFalseFile = new File(dirFile, "jpegfalse.jpg");
					NativeUtil.compressBitmap(bit, quality,
							jpegTrueFile.getAbsolutePath(), true);
					NativeUtil.compressBitmap(bit, quality,
							jpegFalseFile.getAbsolutePath(), false);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

			}
		}).start();
	}
}
