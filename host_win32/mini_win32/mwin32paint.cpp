#include "mwin32paint.h"
#include "mencode.h"
#include "mcontext.h"
#include "mwin32imagefactory.h"

static void PaintClip(Gdiplus::Graphics *pen, MClipGraph *graph)
{
    Gdiplus::RectF rect(
        graph->pixelX(), graph->pixelY(),
        graph->pixelW(), graph->pixelH()
    );
    pen->SetClip(rect, Gdiplus::CombineMode::CombineModeReplace);
}

static void PaintTriangle(Gdiplus::Graphics *pen, MTriangleGraph *graph)
{
    MColorRGBA rgba = { graph->rgba() };
    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    Gdiplus::SolidBrush brush(color);

    Gdiplus::PointF vertices[] = {
        { graph->pixelX0(), graph->pixelY0() },
        { graph->pixelX1(), graph->pixelY1() },
        { graph->pixelX2(), graph->pixelY2() },
    };

    pen->FillPolygon(&brush, vertices, 3);
}

static void PaintImage(Gdiplus::Graphics *pen, MImageGraph *graph)
{
    MImageImpl::ptr impl  = graph->image()->impl();
    auto            wImpl = std::static_pointer_cast<MWin32ImageImpl>(impl);
    Gdiplus::Image *image = wImpl->mReal.get();

    Gdiplus::RectF rect(
        graph->pixelX(), graph->pixelY(),
        graph->pixelW(), graph->pixelH()
    );

    pen->DrawImage(image, rect);
}

static void PaintText(Gdiplus::Graphics *pen, MTextGraph *graph)
{
    //text.
    std::u16string text = MU16StringFromU8(graph->text().c_str());

    //font.
    Gdiplus::Font font(L"MSYH", graph->pixelFontSize());

    //rect.
    Gdiplus::RectF rect(
        graph->pixelX(), graph->pixelY(),
        graph->pixelW(), graph->pixelH()
    );

    //aligment:
    Gdiplus::StringFormat format;

    switch (graph->vAlign())
    {
    case MVAlign::Top   : format.SetLineAlignment(Gdiplus::StringAlignmentNear  ); break;
    case MVAlign::Middle: format.SetLineAlignment(Gdiplus::StringAlignmentCenter); break;
    case MVAlign::Bottom: format.SetLineAlignment(Gdiplus::StringAlignmentFar   ); break;
    }

    switch (graph->hAlign())
    {
    case MHAlign::Left  : format.SetAlignment(Gdiplus::StringAlignmentNear  ); break;
    case MHAlign::Center: format.SetAlignment(Gdiplus::StringAlignmentCenter); break;
    case MHAlign::Right : format.SetAlignment(Gdiplus::StringAlignmentFar   ); break;
    }

    //color:
    MColorRGBA rgba = { graph->rgba() };
    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);

    Gdiplus::SolidBrush brush(color);
    
    //draw.
    pen->DrawString((WCHAR *)text.c_str(), (INT)text.size(), &font, rect, &format, &brush);
}

void MWin32Paint(HDC dc)
{
    Gdiplus::Graphics pen(dc);

    MVector<MGraph::ptr> *graphs = MContextGetGraphs();
    for (auto &graph : graphs->vector)
    {
        switch (graph->type())
        {
        case MGraphType::Clip    : PaintClip    (&pen, (MClipGraph     *)graph.get()); break;
        case MGraphType::Triangle: PaintTriangle(&pen, (MTriangleGraph *)graph.get()); break;
        case MGraphType::Image   : PaintImage   (&pen, (MImageGraph    *)graph.get()); break;
        case MGraphType::Text    : PaintText    (&pen, (MTextGraph     *)graph.get()); break;
        }
    }
}
