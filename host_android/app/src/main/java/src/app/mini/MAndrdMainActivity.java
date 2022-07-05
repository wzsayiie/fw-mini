package src.app.mini;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.util.Timer;
import java.util.TimerTask;

public class MAndrdMainActivity extends Activity
    implements TextWatcher, TextView.OnEditorActionListener, View.OnKeyListener
{
    static {
        System.loadLibrary("main");
    }

    private MAndrdDrawView mMainView;
    private EditText mEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        RelativeLayout layout = new RelativeLayout(this);
        setContentView(layout);
        mMainView = createMainView(layout);
        mEditText = createEditText(layout);

        MAndrdBundle.install(this);
        MAndrdImageFactory.install();

        //pixel scale.
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        setPixelScale(metrics.density);

        //application event.
        appLaunch();
        scheduleTimer(appUpdateSeconds(), MAndrdMainActivity::appUpdate);

        //window event:
        windowLoad();

        mMainView.getViewTreeObserver().addOnGlobalLayoutListener(() -> {
            float width  = mMainView.getMeasuredWidth ();
            float height = mMainView.getMeasuredHeight();
            windowResize(width, height);
        });

        scheduleTimer(windowUpdateSeconds(), () -> {
            mMainView.invalidate();
            updateEditTextState();
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        windowShow();
    }

    @Override
    protected void onStop() {
        super.onStop();
        windowHide();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float activityX = event.getX();
        float activityY = event.getY();

        //NOTE: considering the status bar.
        int[] viewOffset = new int[2];
        mMainView.getLocationInWindow(viewOffset);
        float x = activityX - viewOffset[0];
        float y = activityY - viewOffset[1];

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: windowTouchBegin(x, y); return true;
            case MotionEvent.ACTION_MOVE: windowTouchMove (x, y); return true;
            case MotionEvent.ACTION_UP  : windowTouchEnd  (x, y); return true;

            default: return false;
        }
    }

    private MAndrdDrawView createMainView(RelativeLayout parentLayout) {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(0, 0);
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP   );
        params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        params.addRule(RelativeLayout.ALIGN_PARENT_LEFT  );
        params.addRule(RelativeLayout.ALIGN_PARENT_RIGHT );

        MAndrdDrawView view = new MAndrdDrawView(this);
        view.setLayoutParams(params);
        parentLayout.addView(view);

        return view;
    }

    private EditText createEditText(RelativeLayout parentLayout) {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(800, 100);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);

        EditText edit = new EditText(this);
        edit.setVisibility(View.INVISIBLE);
        //"done" key is only available in single-line mode.
        edit.setSingleLine();
        //finally add listeners. event callbacks may be fired when set the edit.
        edit.addTextChangedListener(this);
        edit.setOnEditorActionListener(this);
        edit.setOnKeyListener(this);

        edit.setLayoutParams(params);
        parentLayout.addView(edit);

        return edit;
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

    private void updateEditTextState() {
        if (!checkWritingUpdated()) {
            return;
        }

        if (checkWritingEnabled()) {
            //set the text.
            String text = checkWritingRawText();
            mEditText.setText(text);
            if (text != null) {
                mEditText.setSelection(0, text.length());
            }

            //show the edit.
            mEditText.setVisibility(View.VISIBLE);
            mEditText.requestFocus();

            //open keyboard.
            InputMethodManager input = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
            input.showSoftInput(mEditText, 0);

        } else {
            //close keyboard.
            InputMethodManager input = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
            input.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);

            //hide the edit.
            mEditText.clearFocus();
            mEditText.setVisibility(View.INVISIBLE);
        }
    }

    public void beforeTextChanged(CharSequence sequence, int start, int count, int after) {}
    public void afterTextChanged(Editable sequence) {}

    public void onTextChanged(CharSequence sequence, int start, int before, int count) {
        //this method monitors the text changes.

        if (mEditText == null) {
            return;
        }

        String string = mEditText.getText().toString();
        windowWrite(string, false);
    }

    public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
        //this method monitors the "done" key on the soft keyboard.

        if (view != mEditText) {
            return false;
        }

        if (actionId == EditorInfo.IME_ACTION_DONE) {
            String string = mEditText.getText().toString();
            windowWrite(string, true);
        }

        //return true to keep soft keyboard (false means that the event has not been consumed yet.
        //if pass the event to the system, the soft keyboard will be hidden).
        return true;
    }

    public boolean onKey(View view, int code, KeyEvent event) {
        //this method monitors the "enter" key of the physical keyboard.

        if (view != mEditText) {
            return false;
        }

        if (code == KeyEvent.KEYCODE_ENTER) {
            if (event.getAction() == KeyEvent.ACTION_UP) {
                String string = mEditText.getText().toString();
                windowWrite(string, true);
            }
            return true;
        }
        return false;
    }

    private static native void    setPixelScale      (float scale);

    private static native float   appUpdateSeconds   ();
    private static native void    appLaunch          ();
    private static native void    appUpdate          ();

    private static native float   windowUpdateSeconds();
    private static native void    windowResize       (float width, float height);
    private static native void    windowLoad         ();
    private static native void    windowShow         ();
    private static native void    windowHide         ();
    private static native void    windowTouchBegin   (float x, float y);
    private static native void    windowTouchMove    (float x, float y);
    private static native void    windowTouchEnd     (float x, float y);
    private static native void    windowWrite        (String text, boolean enter);

    private static native boolean checkWritingUpdated();
    private static native boolean checkWritingEnabled();
    private static native String  checkWritingRawText();
}
