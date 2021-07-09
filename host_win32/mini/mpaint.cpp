#include "mpaint.h"
#include <gdiplus.h>
#include <memory>
#include "mapis.h"
#include "mhostui.h"

static ULONG_PTR sGdiplusToken = 0;

void MPaintStart() {
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&sGdiplusToken, &input, nullptr);
}

void MPaintStop() {
    Gdiplus::GdiplusShutdown(sGdiplusToken);
}

static void PaintTriangle(Gdiplus::Graphics *graphics, int index)
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

    graphics->FillPolygon(brush.get(), vertices, 3);
}

static void PaintImage(Gdiplus::Graphics *graphics, int index)
{
    MImage *image = _MWindowImageObject(index);
    Gdiplus::Image *managedImage = MManagedImage(MImageManagedId(image));

    float x = _MWindowImageX(index);
    float y = _MWindowImageY(index);
    float width  = _MWindowImageWidth (index);
    float height = _MWindowImageHeight(index);

    graphics->DrawImage(managedImage, x, y, width, height);
}

static void PaintLabel(Gdiplus::Graphics *graphics, int index)
{
}

void MPaint(HDC dc)
{
    _MWindowOnDraw();

    std::shared_ptr<Gdiplus::Graphics> graphics(new Gdiplus::Graphics(dc));

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        PaintTriangle(graphics.get(), index);
    }

    int imageCount = _MWindowImageCount();
    for (int index = 0; index < imageCount; ++index) {
        PaintImage(graphics.get(), index);
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        PaintLabel(graphics.get(), index);
    }
}
