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

public class MainActivity extends Activity implements TextWatcher, TextView.OnEditorActionListener, View.OnKeyListener {

    static {
        System.loadLibrary("main");
    }

    private DrawView mDrawView;
    private EditText mEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        RelativeLayout layout = new RelativeLayout(this);
        setContentView(layout);
        mDrawView = createDrawView(layout);
        mEditText = createEditText(layout);

        AndroidApi.registerApi(getApplicationContext());

        //application event:
        appLaunch();

        scheduleTimer(constAppUpdateInterval(), () -> {
            updateEditTextState();
            appUpdate();
        });

        //window event:
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        windowSetPixelDensity(metrics.density);

        windowOnLoad();

        mDrawView.getViewTreeObserver().addOnGlobalLayoutListener(() -> {
            float width  = mDrawView.getMeasuredWidth ();
            float height = mDrawView.getMeasuredHeight();
            windowOnResize(width, height);
        });

        scheduleTimer(constWindowDrawInterval(), () -> mDrawView.invalidate());
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
        mDrawView.getLocationInWindow(viewOffset);

        float x = activityX - viewOffset[0];
        float y = activityY - viewOffset[1];

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: windowOnTouchBegin(x, y); return true;
            case MotionEvent.ACTION_MOVE: windowOnTouchMove (x, y); return true;
            case MotionEvent.ACTION_UP  : windowOnTouchEnd  (x, y); return true;

            default: return false;
        }
    }

    private DrawView createDrawView(RelativeLayout layout) {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(0, 0);
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP   );
        params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        params.addRule(RelativeLayout.ALIGN_PARENT_LEFT  );
        params.addRule(RelativeLayout.ALIGN_PARENT_RIGHT );

        DrawView view = new DrawView(this);
        view.setLayoutParams(params);
        layout.addView(view);

        return view;
    }

    private EditText createEditText(RelativeLayout layout) {
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(800, 100);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);

        EditText edit = new EditText(this);
        edit.setVisibility(View.INVISIBLE);

        //"done" key is only available in single-line mode.
        edit.setSingleLine();

        edit.addTextChangedListener(this);
        edit.setOnEditorActionListener(this);
        edit.setOnKeyListener(this);

        edit.setLayoutParams(params);
        layout.addView(edit);

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
        if (!windowTextBoxUpdated()) {
            return;
        }

        //IMPORTANT: reset the flag.
        windowSetTextBoxUpdated(false);

        if (windowTextBoxEnabled()) {
            String text = windowTextBoxRawString();
            mEditText.setText(text);
            if (text != null) {
                mEditText.setSelection(0, text.length());
            }

            mEditText.setVisibility(View.VISIBLE);
            mEditText.requestFocus();

            InputMethodManager input = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
            input.showSoftInput(mEditText, 0);

        } else {
            InputMethodManager input = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
            input.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);

            mEditText.clearFocus();
            mEditText.setVisibility(View.INVISIBLE);
        }
    }

    public void beforeTextChanged(CharSequence sequence, int start, int count, int after) {}
    public void afterTextChanged(Editable sequence) {}

    public void onTextChanged(CharSequence sequence, int start, int before, int count) {
        //this method monitors the text changes.

        String string = mEditText.getText().toString();
        windowOnTextBox(string, false);
    }

    public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
        //this method monitors the "done" key on the soft keyboard.

        if (view != mEditText) {
            return false;
        }

        if (actionId == EditorInfo.IME_ACTION_DONE) {
            String string = mEditText.getText().toString();
            windowOnTextBox(string, true);
        }

        //return true to keep soft keyboard (false means that the event has not been consumed yet.
        //if pass the event to the system, the soft keyboard will be hidden.).
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
                windowOnTextBox(string, true);
            }
            return true;
        }
        return false;
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

    private native void windowOnTextBox(String string, boolean enter);

    private native boolean windowTextBoxUpdated  ();
    private native boolean windowTextBoxEnabled  ();
    private native String  windowTextBoxRawString();

    private native void windowSetTextBoxUpdated(boolean updated);
}
