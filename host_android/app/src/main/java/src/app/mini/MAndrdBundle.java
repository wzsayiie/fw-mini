package src.app.mini;

import android.annotation.SuppressLint;
import android.content.Context;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MAndrdBundle {

    @SuppressLint("StaticFieldLeak")
    private static Context sContext = null;

    private static native void   installNative();
    private static native String resBundleName();

    public static void install(Context context) {
        sContext = context;
        installNative();
    }

    @SuppressWarnings("unused") //used by native.
    protected static byte[] onLoadResource(String path) {
        try {
            String resPath = resBundleName() + "/" + path;

            InputStream input = sContext.getAssets().open(resPath);
            ByteArrayOutputStream output = new ByteArrayOutputStream();

            byte[] buffer = new byte[4096];
            int length;
            while ((length = input.read(buffer)) != -1) {
                output.write(buffer, 0, length);
            }

            byte[] bytes = output.toByteArray();
            output.close();
            input.close();

            return bytes;

        } catch (IOException e) {
            return null;
        }
    }

    @SuppressWarnings("unused") //used by native.
    protected static String onGetResBundleDirectory() {
        return null;
    }

    @SuppressWarnings("unused") //used by native.
    protected static String onGetDocumentDirectory() {
        return sContext.getFilesDir().getAbsolutePath();
    }

    @SuppressWarnings("unused") //used by native.
    protected static String onGetTemporaryDirectory() {
        return sContext.getCacheDir().getAbsolutePath();
    }
}
