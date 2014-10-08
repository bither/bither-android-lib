/*
 * Copyright 2014 http://Bither.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.testjpg;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import net.bither.util.NativeUtil;

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
							.open("test.jpg");
					Bitmap bit = BitmapFactory.decodeStream(in);
					File dirFile = getExternalCacheDir();
					if (!dirFile.exists()) {
						dirFile.mkdirs();
					}
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
