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
    protected static Bitmap onDecodeBitmap(byte[] bitDat, int width, int height) {
        int[] pixels = collect_Argb_from_rgbA(bitDat);

        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap.setPixels(pixels, 0, width, 0, 0, width, height);
        return bitmap;
    }

    @SuppressWarnings("unused") //used by native.
    protected static byte[] onEncodeFFData(Bitmap bitmap, char format) {
        return null;
    }

    @SuppressWarnings("unused") //used by native.
    protected static byte[] onEncodeBitmap(Bitmap bitmap) {
        int width  = bitmap.getWidth ();
        int height = bitmap.getHeight();

        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        return collect_rgbA_from_Argb(pixels);
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelX(Bitmap bitmap) {
        return bitmap.getWidth();
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelY(Bitmap bitmap) {
        return bitmap.getHeight();
    }

    private static int[] collect_Argb_from_rgbA(byte[] bytes) {
        int   count = bytes.length / 4;
        int[] ints  = new int[count];

        for (int i = 0; i < count; ++i) {
            //NOTE: the color byte order used by c++ side is rgbA, but the android uses Argb.
            int r = bytes[4 * i + 3];
            int g = bytes[4 * i + 2];
            int b = bytes[4 * i + 1];
            int a = bytes[4 * i    ];

            int argb = (
                ((a << 24) & 0xFF000000) |
                ((r << 16) & 0x00FF0000) |
                ((g <<  8) & 0x0000FF00) |
                ((b      ) & 0x000000FF)
            );
            ints[i] = argb;
        }

        return ints;
    }

    private static byte[] collect_rgbA_from_Argb(int[] ints) {
        int    count = ints.length;
        byte[] bytes = new byte[4 * count];

        for (int i = 0; i < count; ++i) {
            int argb = ints[i];

            bytes[4 * i + 3] = (byte) (argb >> 16); //r.
            bytes[4 * i + 2] = (byte) (argb >>  8); //g.
            bytes[4 * i + 1] = (byte) (argb      ); //b.
            bytes[4 * i    ] = (byte) (argb >> 24); //a.
        }

        return bytes;
    }
}
