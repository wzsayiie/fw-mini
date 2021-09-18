#include "mpaint.h"
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
    rgba.rgba = _MWindowTriangleGraphColor(index);

    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    std::shared_ptr<Gdiplus::SolidBrush> brush(new Gdiplus::SolidBrush(color));

    //connect points.
    Gdiplus::PointF vertices[3];
    vertices[0].X = _MWindowTriangleGraphX0(index);
    vertices[0].Y = _MWindowTriangleGraphY0(index);
    vertices[1].X = _MWindowTriangleGraphX1(index);
    vertices[1].Y = _MWindowTriangleGraphY1(index);
    vertices[2].X = _MWindowTriangleGraphX2(index);
    vertices[2].Y = _MWindowTriangleGraphY2(index);

    //draw.
    graphics->FillPolygon(brush.get(), vertices, 3);
}

static void PaintImage(Gdiplus::Graphics *graphics, int index)
{
    //get the image.
    auto imageObject = (MWin32Image *)_MWindowImageGraphObject(index);
    Gdiplus::Image *nativeImage  = imageObject->nativeImage();

    //get the position rectangle.
    float x      = _MWindowImageGraphX     (index);
    float y      = _MWindowImageGraphY     (index);
    float width  = _MWindowImageGraphWidth (index);
    float height = _MWindowImageGraphHeight(index);

    //draw.
    graphics->DrawImage(nativeImage, x, y, width, height);
}

static void PaintLabel(Gdiplus::Graphics *graphics, int index)
{
    //get the string.
    MString *string = _MWindowLabelGraphString(index);
    auto     wchars = (const WCHAR *)MStringU16Chars(string);
    int      wsize  = MStringU16Size(string);

    //set the font.
    _MPixel fontSize = _MWindowLabelGraphFontSize(index);
    std::shared_ptr<Gdiplus::Font> font(new Gdiplus::Font(L"MSYH", fontSize));

    //set the position rectangle:
    _MPixel x      = _MWindowLabelGraphX     (index);
    _MPixel y      = _MWindowLabelGraphY     (index);
    _MPixel width  = _MWindowLabelGraphWidth (index);
    _MPixel height = _MWindowLabelGraphHeight(index);

    Gdiplus::RectF rect(x, y, width, height);

    //set the alignments:
    std::shared_ptr<Gdiplus::StringFormat> format(new Gdiplus::StringFormat());
    
    switch (_MWindowLabelGraphHAlign(index))
    {
        case MHAlign_Left  : format->SetAlignment(Gdiplus::StringAlignmentNear  ); break;
        case MHAlign_Center: format->SetAlignment(Gdiplus::StringAlignmentCenter); break;
        case MHAlign_Right : format->SetAlignment(Gdiplus::StringAlignmentFar   ); break;
        default:;
    }

    switch (_MWindowLabelGraphVAlign(index))
    {
        case MVAlign_Top   : format->SetLineAlignment(Gdiplus::StringAlignmentNear  ); break;
        case MVAlign_Center: format->SetLineAlignment(Gdiplus::StringAlignmentCenter); break;
        case MVAlign_Bottom: format->SetLineAlignment(Gdiplus::StringAlignmentFar   ); break;
        default:;
    }

    //set the color:
    MColorPattern rgba;
    rgba.rgba = _MWindowLabelGraphColor(index);

    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    std::shared_ptr<Gdiplus::Brush> brush(new Gdiplus::SolidBrush(color));

    //draw.
    graphics->DrawString(wchars, wsize, font.get(), rect, format.get(), brush.get());
}

void MPaint(HDC dc)
{
    _MWindowOnDraw();

    std::shared_ptr<Gdiplus::Graphics> graphics(new Gdiplus::Graphics(dc));

    int count = _MWindowGraphCount();
    for (int index = 0; index < count; ++index)
    {
        _MGraph graph = _MWindowGraphType(index);

        switch (graph)
        {
            case _MGraph_Triangle: PaintTriangle(graphics.get(), index); break;
            case _MGraph_Image   : PaintImage   (graphics.get(), index); break;
            case _MGraph_Label   : PaintLabel   (graphics.get(), index); break;
            default:;
        }
    }
}
