#include "mpaint.h"
#include <gdiplus.h>
#include <memory>
#include "mhostui.h"
#include "mwinapi.h"

static ULONG_PTR sGdiplusToken = 0;

void MPaintStart()
{
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&sGdiplusToken, &input, nullptr);
}

void MPaintStop()
{
    Gdiplus::GdiplusShutdown(sGdiplusToken);
}

static void PaintTriangle(Gdiplus::Graphics *graphics, int index)
{
    //set the color:
    MColorPattern rgba;
    rgba.rgba = _MWindowTriangleColor(index);

    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    std::shared_ptr<Gdiplus::SolidBrush> brush(new Gdiplus::SolidBrush(color));

    //connect points.
    Gdiplus::PointF vertices[3];
    for (int n = 0; n < 3; ++n)
    {
        vertices[n].X = _MWindowTriangleVertexX(index, n);
        vertices[n].Y = _MWindowTriangleVertexY(index, n);
    }

    //draw.
    graphics->FillPolygon(brush.get(), vertices, 3);
}

static void PaintImage(Gdiplus::Graphics *graphics, int index)
{
    //get the image.
    MImage *image = _MWindowImageObject(index);
    Gdiplus::Image *managedImage = MManagedImage(MImageManagedId(image));

    //get the position rectangle.
    float x = _MWindowImageX(index);
    float y = _MWindowImageY(index);
    float width  = _MWindowImageWidth (index);
    float height = _MWindowImageHeight(index);

    //draw.
    graphics->DrawImage(managedImage, x, y, width, height);
}

static void PaintLabel(Gdiplus::Graphics *graphics, int index)
{
    //get the string.
    MString *string = _MWindowLabelString(index);
    auto    *wchars = (const WCHAR *)MStringU16Chars(string);
    int      wsize  = MStringU16Size(string);

    //set the font.
    _MPixel fontSize = _MWindowLabelFontSize(index);
    std::shared_ptr<Gdiplus::Font> font(new Gdiplus::Font(L"MSYH", fontSize));

    //set the position rectangle.
    _MPixel x = _MWindowLabelX(index);
    _MPixel y = _MWindowLabelY(index);

    _MPixel width  = _MWindowLabelWidth (index);
    _MPixel height = _MWindowLabelHeight(index);

    Gdiplus::RectF rect(x, y, width, height);

    //set the alignments:
    std::shared_ptr<Gdiplus::StringFormat> format(new Gdiplus::StringFormat());
    
    switch (_MWindowLabelHAlign(index)) {
        case MHAlign_Left  : format->SetAlignment(Gdiplus::StringAlignmentNear  ); break;
        case MHAlign_Center: format->SetAlignment(Gdiplus::StringAlignmentCenter); break;
        case MHAlign_Right : format->SetAlignment(Gdiplus::StringAlignmentFar   ); break;
        default:;
    }

    switch (_MWindowLabelVAlign(index)) {
        case MVAlign_Top   : format->SetLineAlignment(Gdiplus::StringAlignmentNear  ); break;
        case MVAlign_Center: format->SetLineAlignment(Gdiplus::StringAlignmentCenter); break;
        case MVAlign_Bottom: format->SetLineAlignment(Gdiplus::StringAlignmentFar   ); break;
        default:;
    }

    //set the color:
    MColorPattern rgba;
    rgba.rgba = _MWindowLabelColor(index);

    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    std::shared_ptr<Gdiplus::Brush> brush(new Gdiplus::SolidBrush(color));

    //draw.
    graphics->DrawString(wchars, wsize, font.get(), rect, format.get(), brush.get());
}

void MPaint(HDC dc)
{
    _MWindowOnDraw();

    std::shared_ptr<Gdiplus::Graphics> graphics(new Gdiplus::Graphics(dc));

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index)
    {
        PaintTriangle(graphics.get(), index);
    }

    int imageCount = _MWindowImageCount();
    for (int index = 0; index < imageCount; ++index)
    {
        PaintImage(graphics.get(), index);
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index)
    {
        PaintLabel(graphics.get(), index);
    }
}
