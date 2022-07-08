package src.app.mini;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class MAndrdImageFactory {

    public static native void install();

    @SuppressWarnings("unused") //used by native.
    protected static Bitmap onDecodeFFData(byte[] ffData) {
        return BitmapFactory.decodeByteArray(ffData, 0, ffData.length);
    }

    @SuppressWarnings("unused") //used by native.
    protected static Bitmap onDecodeBitmap(int[] bitDat, int width, int height) {
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap.setPixels(bitDat, 0, width, 0, 0, width, height);
        return bitmap;
    }

    @SuppressWarnings("unused") //used by native.
    protected static byte[] onEncodeFFData(Bitmap bitmap, char format) {
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        switch (format) {
            case 'J': bitmap.compress(Bitmap.CompressFormat.JPEG, 100, output); break;
            case 'P': bitmap.compress(Bitmap.CompressFormat.PNG , 100, output); break;
        }

        try {
            byte[] ffData = output.toByteArray();
            output.close();

            if (ffData.length > 0) {
                return ffData;
            }
            return null;

        } catch (IOException e) {
            return null;
        }
    }

    @SuppressWarnings("unused") //used by native.
    protected static int[] onEncodeBitmap(Bitmap bitmap) {
        int width  = bitmap.getWidth ();
        int height = bitmap.getHeight();

        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        return pixels;
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelX(Bitmap bitmap) {
        return bitmap.getWidth();
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelY(Bitmap bitmap) {
        return bitmap.getHeight();
    }
}
