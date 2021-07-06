#include "mpaint.h"
#include <gdiplus.h>
#include "mhostui.h"

static ULONG_PTR sGdiplusToken = 0;

void MPaintStart() {
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&sGdiplusToken, &input, nullptr);
}

void MPaintStop() {
    Gdiplus::GdiplusShutdown(sGdiplusToken);
}

static void PaintTriangle(HDC dc, int index)
{
    MColorPattern rgba = {0};
    rgba.rgba = _MWindowTriangleColor(index);

    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    std::shared_ptr<Gdiplus::SolidBrush> brush(new Gdiplus::SolidBrush(color));

    Gdiplus::PointF vertices[3];
    for (int n = 0; n < 3; ++n)
    {
        vertices[n].X = _MWindowTriangleVertexX(index, n);
        vertices[n].Y = _MWindowTriangleVertexY(index, n);
    }

    std::shared_ptr<Gdiplus::Graphics> graphics(new Gdiplus::Graphics(dc));
    graphics->FillPolygon(brush.get(), vertices, 3);
}

static void PaintLabel(HDC dc, int index)
{
}

void MPaint(HDC dc)
{
    _MWindowOnDraw();

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        PaintTriangle(dc, index);
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        PaintLabel(dc, index);
    }
}
