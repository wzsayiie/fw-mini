package src.app.mini;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
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
    }

    private void drawTriangle(int index, Canvas canvas) {

        Paint paint = new Paint();
        paint.setColor(windowTriangleColor(index));

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

        canvas.drawPath(path, paint);
    }

    private native void windowOnDraw();

    private native int   windowTriangleCount  ();
    private native float windowTriangleVertexX(int index, int vertexIndex);
    private native float windowTriangleVertexY(int index, int vertexIndex);
    private native int   windowTriangleColor  (int index);
}
