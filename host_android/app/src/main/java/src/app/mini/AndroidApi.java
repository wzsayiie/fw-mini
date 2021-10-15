package src.app.mini;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class AndroidApi {

    @SuppressLint("StaticFieldLeak")
    private static Context sContext = null;

    public static void registerApi(Context context) {
        sContext = context;
        registerAndroidApi();
    }

    private static native void registerAndroidApi();
    private static native String assetBundleName();

    protected static void printMessage(String string) {
        Log.i("mmm", string);
    }

    protected static byte[] copyBundleAsset(String path) {
        try {
            String assetPath = assetBundleName() + "/" + path;

            InputStream input = sContext.getAssets().open(assetPath);
            ByteArrayOutputStream output = new ByteArrayOutputStream();

            byte[] buffer = new byte[4096];
            int length;
            while ((length = input.read(buffer)) != -1) {
                output.write(buffer, 0, length);
            }

            byte[] data = output.toByteArray();
            output.close();
            input.close();

            return data;

        } catch (IOException e) {
            return null;
        }
    }

    protected static Bitmap createImage(byte[] data) {
        if (data != null) {
            return BitmapFactory.decodeByteArray(data, 0, data.length);
        }
        return null;
    }

    protected static String copyDocumentPath() {
        return sContext.getFilesDir().getAbsolutePath();
    }

    protected static String copyCachePath() {
        return sContext.getFilesDir().getAbsolutePath();
    }

    protected static String copyTemporaryPath() {
        return sContext.getFilesDir().getAbsolutePath();
    }

    protected static boolean makeDirectory(String path) {
        File file = new File(path);
        return file.mkdirs();
    }

    protected static String[] copyPathSubItems(String path) {
        File file = new File(path);
        return file.list();
    }

    protected static void removePath(String path) {
        File file = new File(path);
        if (file.exists()) {
            removeRecursively(file);
        }
    }

    private static void removeRecursively(File file) {
        if (file.isDirectory()) {
            File[] subFiles = file.listFiles();
            if (subFiles != null) {
                for (File it : subFiles) {
                    removeRecursively(it);
                }
            }
        } else if (file.isFile()) {
            file.delete();
        }
    }

    protected static boolean pathExists(String path) {
        File file = new File(path);
        return file.exists();
    }

    protected static boolean directoryExists(String path) {
        File file = new File(path);
        return file.exists() && file.isDirectory();
    }

    protected static boolean fileExists(String path) {
        File file = new File(path);
        return file.exists() && file.isFile();
    }
}
