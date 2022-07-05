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
        int[] pixels = collectPixelInts(bitDat);

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
        return collectPixelBytes(pixels);
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelX(Bitmap bitmap) {
        return bitmap.getWidth();
    }

    @SuppressWarnings("unused") //used by native.
    protected static float onGetPixelY(Bitmap bitmap) {
        return bitmap.getHeight();
    }

    private static int[] collectPixelInts(byte[] bytes) {
        int   count = bytes.length / 4;
        int[] ints  = new int[count];

        for (int i = 0; i < count; ++i) {
            //NOTE: the color byte order used by android is Argb, but the C++ side uses rgbA.
            int a = bytes[4 * i + 3];
            int r = bytes[4 * i + 2];
            int g = bytes[4 * i + 1];
            int b = bytes[4 * i    ];

            int rgba = (
                ((r << 24) & 0xFF000000) |
                ((g << 16) & 0x00FF0000) |
                ((b <<  8) & 0x0000FF00) |
                ((a      ) & 0x000000FF)
            );
            ints[i] = rgba;
        }

        return ints;
    }

    private static byte[] collectPixelBytes(int[] ints) {
        int    count = ints.length;
        byte[] bytes = new byte[4 * count];

        for (int i = 0; i < count; ++i) {
            int rgba = ints[i];

            bytes[4 * i + 3] = (byte) (rgba      ); //a.
            bytes[4 * i + 2] = (byte) (rgba >> 24); //r.
            bytes[4 * i + 1] = (byte) (rgba >> 16); //g.
            bytes[4 * i    ] = (byte) (rgba >>  8); //b.
        }

        return bytes;
    }
}
