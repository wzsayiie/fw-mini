package src.app.mini;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.view.View;

public class DrawView extends View {

    public DrawView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.save();

        windowOnDraw();

        int count = windowGraphCount();
        for (int index = 0; index < count; ++index) {
            int graph = windowGraphType(index);

            switch (graph) {
                case 'C': drawClip    (index, canvas); break;
                case 'T': drawTriangle(index, canvas); break;
                case 'I': drawImage   (index, canvas); break;
                case 'L': drawLabel   (index, canvas); break;
            }
        }
    }

    private void drawClip(int index, Canvas canvas) {
        float x = windowClipGraphX     (index);
        float y = windowClipGraphY     (index);
        float w = windowClipGraphWidth (index);
        float h = windowClipGraphHeight(index);

        canvas.restore();
        canvas.save();
        canvas.clipRect(x, y, x + w, y + h);
    }

    private void drawTriangle(int index, Canvas canvas) {
        //set the color.
        Paint paint = new Paint();
        paint.setColor(windowTriangleGraphColor(index));

        //connect the points.
        float x0 = windowTriangleGraphX0(index);
        float y0 = windowTriangleGraphY0(index);
        float x1 = windowTriangleGraphX1(index);
        float y1 = windowTriangleGraphY1(index);
        float x2 = windowTriangleGraphX2(index);
        float y2 = windowTriangleGraphY2(index);

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
        Bitmap image = windowImageGraphObject(index);

        //set the position rectangle.
        int x = (int) windowImageGraphX     (index);
        int y = (int) windowImageGraphY     (index);
        int w = (int) windowImageGraphWidth (index);
        int h = (int) windowImageGraphHeight(index);

        Rect srcRect = new Rect(0, 0, image.getWidth(), image.getHeight());
        Rect dstRect = new Rect(x, y, x + w, y + h);

        //draw.
        canvas.drawBitmap(image, srcRect, dstRect, null);
    }

    private void drawLabel(int index, Canvas canvas) {
        //get the string.
        String text = windowLabelGraphString(index);

        //set the color.
        Paint paint = new Paint();
        paint.setColor(windowLabelGraphColor(index));

        //set the font size.
        paint.setTextSize(windowLabelGraphFontSize(index));

        //set the position rectangle.
        float x = windowLabelGraphX     (index);
        float y = windowLabelGraphY     (index);
        float w = windowLabelGraphWidth (index);
        float h = windowLabelGraphHeight(index);

        Rect textRect = new Rect();
        paint.getTextBounds(text, 0, text.length(), textRect);

        float tw = textRect.width ();           //the text width.
        float th = textRect.height() * 1.2f;    //the text height.
        float bh = textRect.height();           //the height above text baseline.

        int hAlign = windowLabelGraphHAlign(index);
        int vAlign = windowLabelGraphVAlign(index);

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

    private native int windowGraphCount();
    private native int windowGraphType (int index);

    private native float  windowClipGraphX        (int index);
    private native float  windowClipGraphY        (int index);
    private native float  windowClipGraphWidth    (int index);
    private native float  windowClipGraphHeight   (int index);

    private native float  windowTriangleGraphX0   (int index);
    private native float  windowTriangleGraphY0   (int index);
    private native float  windowTriangleGraphX1   (int index);
    private native float  windowTriangleGraphY1   (int index);
    private native float  windowTriangleGraphX2   (int index);
    private native float  windowTriangleGraphY2   (int index);
    private native int    windowTriangleGraphColor(int index);

    private native Bitmap windowImageGraphObject  (int index);
    private native float  windowImageGraphX       (int index);
    private native float  windowImageGraphY       (int index);
    private native float  windowImageGraphWidth   (int index);
    private native float  windowImageGraphHeight  (int index);

    private native String windowLabelGraphString  (int index);
    private native int    windowLabelGraphColor   (int index);
    private native float  windowLabelGraphFontSize(int index);
    private native int    windowLabelGraphHAlign  (int index);
    private native int    windowLabelGraphVAlign  (int index);
    private native float  windowLabelGraphX       (int index);
    private native float  windowLabelGraphY       (int index);
    private native float  windowLabelGraphWidth   (int index);
    private native float  windowLabelGraphHeight  (int index);
}
