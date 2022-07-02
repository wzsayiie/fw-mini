package src.app.mini;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.view.View;

public class MAndrdDrawView extends View {

    public MAndrdDrawView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        //NOTE: save original canvas without specified clip.
        canvas.save();

        drawGraphs();

        int count = graphCount();
        for (int index = 0; index < count; ++index) {
            selectGraph(index);

            switch (graphType()) {
                case 'C': drawClip   (canvas); break;
                case 'P': drawPolygon(canvas); break;
                case 'I': drawImage  (canvas); break;
                case 'T': drawText   (canvas); break;
            }
        }
    }

    private void drawClip(Canvas canvas) {
        //restore original canvas without specified clip.
        canvas.restore();
        canvas.save();

        float x = clipX     ();
        float y = clipY     ();
        float w = clipWidth ();
        float h = clipHeight();

        canvas.clipRect(x, y, x + w, y + h);
    }

    private void drawPolygon(Canvas canvas) {
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

    private void drawImage(Canvas canvas) {
        //get the image.
        Bitmap image = imageObject();

        //set the position rectangle.
        int x = (int) imageX     ();
        int y = (int) imageY     ();
        int w = (int) imageWidth ();
        int h = (int) imageHeight();

        Rect samplingRect = new Rect(0, 0, image.getWidth(), image.getHeight());
        Rect targetRect   = new Rect(x, y, x + w, y + h);

        //draw.
        canvas.drawBitmap(image, samplingRect, targetRect, null);
    }

    private void drawText(Canvas canvas) {
        //get the string.
        String str = textString();

        //set the color.
        Paint paint = new Paint();
        paint.setColor(textColor());

        //set the font size.
        paint.setTextSize(textFontSize());

        //set the position rectangle.
        float x = textX     ();
        float y = textY     ();
        float w = textWidth ();
        float h = textHeight();

        Rect textRect = new Rect();
        paint.getTextBounds(str, 0, str.length(), textRect);

        float tw = textRect.width ();           //the text width.
        float th = textRect.height() * 1.2f;    //the text height.
        float bh = textRect.height();           //the height above text baseline.

        int hAlign = textHAlign();
        int vAlign = textVAlign();

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
        canvas.drawText(str, tx, ty + bh, paint);
    }

    private native void   drawGraphs    ();

    private native int    graphCount    ();
    private native void   selectGraph   (int index);
    private native char   graphType     ();

    private native float  clipX         ();
    private native float  clipY         ();
    private native float  clipWidth     ();
    private native float  clipHeight    ();

    private native int    polygonVerties();
    private native float  polygonVertexX(int index);
    private native float  polygonVertexY(int index);
    private native int    polygonRGBA   ();

    private native Bitmap imageObject   ();
    private native float  imageX        ();
    private native float  imageY        ();
    private native float  imageWidth    ();
    private native float  imageHeight   ();

    private native String textString    ();
    private native float  textFontSize  ();
    private native int    textColor     ();
    private native char   textHAlign    ();
    private native char   textVAlign    ();
    private native float  textX         ();
    private native float  textY         ();
    private native float  textWidth     ();
    private native float  textHeight    ();
}
