package src.app.mini;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

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
        return null;
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
