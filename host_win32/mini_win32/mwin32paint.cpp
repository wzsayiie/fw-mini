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

static void PaintPolygon(Gdiplus::Graphics *pen, MPolygonGraph *graph)
{
    MColorRGBA rgba = { graph->rgba() };
    Gdiplus::Color color(rgba.alpha, rgba.red, rgba.green, rgba.blue);
    Gdiplus::SolidBrush brush(color);

    std::vector<Gdiplus::PointF> vertices;
    int points = graph->points();
    for (int index = 0; index < points; ++index)
    {
        float x = graph->pixelX(index);
        float y = graph->pixelY(index);

        vertices.push_back(Gdiplus::PointF(x, y));
    }

    pen->FillPolygon(&brush, vertices.data(), points);
}

static void PaintImage(Gdiplus::Graphics *pen, MImageGraph *graph)
{
    MImage::ptr      image    = graph->image();
    MWin32Image::ptr w32image = std::static_pointer_cast<MWin32Image>(image);
    Gdiplus::Image  *gdiImage = w32image->mGdiImage.get();

    Gdiplus::RectF rect(
        graph->pixelX(), graph->pixelY(),
        graph->pixelW(), graph->pixelH()
    );

    pen->DrawImage(gdiImage, rect);
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
    for (const auto &graph : *graphs)
    {
        switch (graph->type())
        {
        case MGraphType::Clip   : PaintClip   (&pen, (MClipGraph    *)graph.get()); break;
        case MGraphType::Polygon: PaintPolygon(&pen, (MPolygonGraph *)graph.get()); break;
        case MGraphType::Image  : PaintImage  (&pen, (MImageGraph   *)graph.get()); break;
        case MGraphType::Text   : PaintText   (&pen, (MTextGraph    *)graph.get()); break;
        }
    }
}
