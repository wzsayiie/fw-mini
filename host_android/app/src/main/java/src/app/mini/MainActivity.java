package src.app.mini;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.view.MotionEvent;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("main");
    }

    private MainView mMainView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mMainView = new MainView(this);
        setContentView(mMainView);

        //application event.
        appLaunch();
        scheduleTimer(constAppUpdateInterval(), this::appUpdate);

        //window event:
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        windowSetPixelDensity(metrics.density);

        windowOnLoad();

        mMainView.getViewTreeObserver().addOnGlobalLayoutListener(() -> {
            float width  = mMainView.getMeasuredWidth ();
            float height = mMainView.getMeasuredHeight();
            windowOnResize(width, height);
        });

        scheduleTimer(constWindowDrawInterval(), () -> mMainView.invalidate());
    }

    @Override
    protected void onStart() {
        super.onStart();
        windowOnShow();
    }

    @Override
    protected void onStop() {
        super.onStop();
        windowOnHide();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float activityX = event.getX();
        float activityY = event.getY();

        int[] viewOffset = new int[2];
        mMainView.getLocationInWindow(viewOffset);

        float x = activityX - viewOffset[0];
        float y = activityY - viewOffset[1];

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: windowOnTouchBegin(x, y); return true;
            case MotionEvent.ACTION_MOVE: windowOnTouchMove (x, y); return true;
            case MotionEvent.ACTION_UP  : windowOnTouchEnd  (x, y); return true;

            default: return false;
        }
    }

    private void scheduleTimer(float intervalSeconds, Runnable runnable) {
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                new Handler(getMainLooper()).post(runnable);
            }
        };
        new Timer().schedule(task, 0, (long)(1000 * intervalSeconds));
    }

    private native float constAppUpdateInterval ();
    private native float constWindowDrawInterval();

    private native void appLaunch();
    private native void appUpdate();

    private native void windowSetPixelDensity(float density);

    private native void windowOnLoad();
    private native void windowOnShow();
    private native void windowOnHide();

    private native void windowOnResize(float width, float height);

    private native void windowOnTouchBegin(float x, float y);
    private native void windowOnTouchMove (float x, float y);
    private native void windowOnTouchEnd  (float x, float y);
}
