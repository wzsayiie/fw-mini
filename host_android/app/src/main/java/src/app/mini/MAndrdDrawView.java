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

        clearGraphs();
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
        //color.
        Paint paint = new Paint();
        paint.setColor(polygonARGB());

        //points:
        Path path = new Path();

        int points = polygonPoints();
        for (int i = 0; i < points; ++i) {
            float x = polygonPointX(i);
            float y = polygonPointY(i);

            if (i == 0) {
                path.moveTo(x, y);
            } else {
                path.lineTo(x, y);
            }
        }

        path.close();

        //draw.
        canvas.drawPath(path, paint);
    }

    private void drawImage(Canvas canvas) {
        //image.
        Bitmap image = imageObject();

        //alpha.
        Paint paint = new Paint();
        paint.setAlpha((int)(imageAlpha() * 255.f));

        //area:
        int x = (int) imageX     ();
        int y = (int) imageY     ();
        int w = (int) imageWidth ();
        int h = (int) imageHeight();

        Rect samplingRect = new Rect(0, 0, image.getWidth(), image.getHeight());
        Rect targetRect   = new Rect(x, y, x + w, y + h);

        //draw.
        canvas.drawBitmap(image, samplingRect, targetRect, paint);
    }

    private void drawText(Canvas canvas) {
        //string.
        String str = textString();

        //font size.
        Paint paint = new Paint();
        paint.setTextSize(textFontSize());

        //color.
        paint.setColor(textARGB());

        //area:
        float x = textX     ();
        float y = textY     ();
        float w = textWidth ();
        float h = textHeight();

        Rect textRect = new Rect();
        paint.getTextBounds(str, 0, str.length(), textRect);

        float tw = textRect.width ();           //the text width.
        float th = textRect.height() * 1.2f;    //the text height.
        float bh = textRect.height();           //the height above text baseline.

        //align:
        int hAlign = textHAlign();
        int vAlign = textVAlign();

        float tx = 0;
        float ty = 0;
        if /**/ (hAlign == 'L') { tx = x               ; }
        else if (hAlign == 'C') { tx = x + (w - tw) / 2; }
        else if (hAlign == 'R') { tx = x + (w - tw)    ; }
        if /**/ (vAlign == 'T') { ty = y               ; }
        else if (vAlign == 'M') { ty = y + (h - th) / 2; }
        else if (vAlign == 'B') { ty = y + (h - th)    ; }

        //draw.
        //NOTE: drawText() requires starting point of the text baseline.
        canvas.drawText(str, tx, ty + bh, paint);
    }

    private static native void   drawGraphs   ();
    private static native void   clearGraphs  ();

    private static native int    graphCount   ();
    private static native void   selectGraph  (int index);
    private static native char   graphType    ();

    private static native float  clipX        ();
    private static native float  clipY        ();
    private static native float  clipWidth    ();
    private static native float  clipHeight   ();

    private static native int    polygonPoints();
    private static native float  polygonPointX(int index);
    private static native float  polygonPointY(int index);
    private static native int    polygonARGB  ();

    private static native Bitmap imageObject  ();
    private static native float  imageAlpha   ();
    private static native float  imageX       ();
    private static native float  imageY       ();
    private static native float  imageWidth   ();
    private static native float  imageHeight  ();

    private static native String textString   ();
    private static native float  textFontSize ();
    private static native int    textARGB     ();
    private static native char   textHAlign   ();
    private static native char   textVAlign   ();
    private static native float  textX        ();
    private static native float  textY        ();
    private static native float  textWidth    ();
    private static native float  textHeight   ();
}
