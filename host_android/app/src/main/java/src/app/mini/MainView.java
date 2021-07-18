package src.app.mini;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.view.View;

public class MainView extends View {

    public MainView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        windowOnDraw();

        int triangleCount = windowTriangleCount();
        for (int index = 0; index < triangleCount; ++index) {
            drawTriangle(index, canvas);
        }

        int imageCount = windowImageCount();
        for (int index = 0; index < imageCount; ++index) {
            drawImage(index, canvas);
        }

        int labelCount = windowLabelCount();
        for (int index = 0; index < labelCount; ++index) {
            drawLabel(index, canvas);
        }
    }

    private void drawTriangle(int index, Canvas canvas) {
        //set the color.
        Paint paint = new Paint();
        paint.setColor(windowTriangleColor(index));

        //connect the points.
        float x0 = windowTriangleVertexX(index, 0);
        float y0 = windowTriangleVertexY(index, 0);
        float x1 = windowTriangleVertexX(index, 1);
        float y1 = windowTriangleVertexY(index, 1);
        float x2 = windowTriangleVertexX(index, 2);
        float y2 = windowTriangleVertexY(index, 2);

        Path path = new Path();
        path.moveTo(x0, y0);
        path.lineTo(x1, y1);
        path.lineTo(x2, y2);
        path.close();

        //draw.
        canvas.drawPath(path, paint);
    }

    private void drawImage(int index, Canvas canvas) {
        //get the image.
        int managedId = windowImageObject(index);
        Bitmap image = AndroidApi.getManagedImage(managedId);

        //set the position rectangle.
        int x = (int) windowImageX(index);
        int y = (int) windowImageY(index);
        int w = (int) windowImageWidth (index);
        int h = (int) windowImageHeight(index);

        Rect srcRect = new Rect(0, 0, w, h);
        Rect dstRect = new Rect(x, y, x + w, y + h);

        //draw.
        canvas.drawBitmap(image, srcRect, dstRect, null);
    }

    private void drawLabel(int index, Canvas canvas) {
        //get the string.
        String text = windowLabelString(index);

        //set the color.
        Paint paint = new Paint();
        paint.setColor(windowLabelColor(index));

        //set the font size.
        paint.setTextSize(windowLabelFontSize(index));

        //set the position rectangle.
        float x = windowLabelX(index);
        float y = windowLabelY(index);
        float w = windowLabelWidth (index);
        float h = windowLabelHeight(index);

        Rect textRect = new Rect();
        paint.getTextBounds(text, 0, text.length(), textRect);

        float tw = textRect.width ();           //the text width.
        float th = textRect.height() * 1.2f;    //the text height.
        float bh = textRect.height();           //the height above text baseline.

        int hAlign = windowLabelHAlign(index);
        int vAlign = windowLabelVAlign(index);

        float tx = 0;
        float ty = 0;
        if /**/ (hAlign == 'L') { tx = x; }
        else if (hAlign == 'C') { tx = x + (w - tw) / 2; }
        else if (hAlign == 'R') { tx = x + (w - tw); }
        if /**/ (vAlign == 'T') { ty = y; }
        else if (vAlign == 'C') { ty = y + (h - th) / 2; }
        else if (vAlign == 'B') { ty = y + (h - th); }

        //draw.
        //NOTE: drawText() requires starting point of the text baseline.
        canvas.drawText(text, tx, ty + bh, paint);
    }

    private native void windowOnDraw();

    private native int   windowTriangleCount  ();
    private native float windowTriangleVertexX(int index, int vertexIndex);
    private native float windowTriangleVertexY(int index, int vertexIndex);
    private native int   windowTriangleColor  (int index);

    private native int   windowImageCount ();
    private native int   windowImageObject(int index);
    private native float windowImageX     (int index);
    private native float windowImageY     (int index);
    private native float windowImageWidth (int index);
    private native float windowImageHeight(int index);
    
    private native int    windowLabelCount   ();
    private native String windowLabelString  (int index);
    private native int    windowLabelColor   (int index);
    private native float  windowLabelFontSize(int index);
    private native int    windowLabelHAlign  (int index);
    private native int    windowLabelVAlign  (int index);
    private native float  windowLabelX       (int index);
    private native float  windowLabelY       (int index);
    private native float  windowLabelWidth   (int index);
    private native float  windowLabelHeight  (int index);
}
