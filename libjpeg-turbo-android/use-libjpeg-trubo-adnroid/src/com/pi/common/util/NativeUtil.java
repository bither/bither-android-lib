package com.pi.common.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.Log;

public class NativeUtil {
	public static void compressBitmap(Bitmap bit, int quality, String fileName,
			boolean optimize) {

		// boolean checkSdk16 = ((StringUtil.compareString(Build.DEVICE,
		// "bbk77_cu_jb")
		// || StringUtil.compareString(Build.DEVICE, "bird77_a_twn_jb")
		// || StringUtil.compareString(Build.DEVICE, "n821") || StringUtil
		// .compareString(Build.DEVICE, "hwG520-5000")) && Build.VERSION.SDK_INT
		// == 16);
		// if (checkSdk16) {
		// try {
		// File file = new File(fileName);
		// bit.compress(CompressFormat.JPEG, 70,
		// new FileOutputStream(file));
		// LogUtil.d("native", "compress of bitmap");
		// } catch (FileNotFoundException e) {
		// e.printStackTrace();
		// }
		// return;
		// }

		Log.d("native", "compress of native");
		if (bit.getConfig() != Config.ARGB_8888) {
			Bitmap result = null;

			result = Bitmap.createBitmap(bit.getWidth(), bit.getHeight(),
					Config.ARGB_8888);
			Canvas canvas = new Canvas(result);
			Rect rect = new Rect(0, 0, bit.getWidth(), bit.getHeight());
			canvas.drawBitmap(bit, null, rect, null);
			saveBitmap(result, quality, fileName, optimize);
			result.recycle();
		} else {
			saveBitmap(bit, quality, fileName, optimize);
		}

	}

	private static void saveBitmap(Bitmap bit, int quality, String fileName,
			boolean optimize) {
		compressBitmap(bit, bit.getWidth(), bit.getHeight(), quality,
				fileName.getBytes(), optimize);

	}

	public static boolean isInteger(Object obj) {
		try {
			Integer.parseInt(obj.toString());
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	private static native String compressBitmap(Bitmap bit, int w, int h,
			int quality, byte[] fileNameBytes, boolean optimize);

	static {
		System.loadLibrary("jpegpi");
		System.loadLibrary("pijni");

	}

}
