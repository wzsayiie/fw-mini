#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import "mcontext.h"
#import "mwindow.h"

@implementation MMACDrawView

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
    #if TARGET_OS_IOS
        //by default, "UIView" will not clear the buffer before redrawing.
        self.backgroundColor = UIColor.whiteColor;
    #endif
    }
    return self;
}

- (void)drawRect:(CGRect)dirtyRect {
    MWindow::mainWindow()->draw();
    
#if TARGET_OS_OSX
    CGContextRef ctx = NSGraphicsContext.currentContext.CGContext;
#elif TARGET_OS_IOS
    CGContextRef ctx = UIGraphicsGetCurrentContext();
#endif
    CGFloat height = self.frame.size.height;
    
    MVector<MGraph::ptr> *graphs = MContextGetGraphs();
    for (const auto &g : *graphs) {
        switch (g->type()) {
            case MGraphType::Clip   : [self drawClip   :(MClipGraph    *)g.get() withContext:ctx viewHeight:height]; break;
            case MGraphType::Polygon: [self drawPolygon:(MPolygonGraph *)g.get() withContext:ctx viewHeight:height]; break;
            case MGraphType::Image  : [self drawImage  :(MImageGraph   *)g.get() withContext:ctx viewHeight:height]; break;
            case MGraphType::Text   : [self drawText   :(MTextGraph    *)g.get() withContext:ctx viewHeight:height]; break;
        }
    }
    
    MContextReset();
}

- (void)drawClip:(MClipGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    CGFloat w = graph->pixelW();
    CGFloat h = graph->pixelH();
    CGFloat x = graph->pixelX();
#if TARGET_OS_OSX
    //on mac os, the origin of a rectangle is the lower left corner.
    CGFloat y = viewHeight - h - graph->pixelY();
#elif TARGET_OS_IOS
    CGFloat y = graph->pixelY();
#endif
    
    CGContextResetClip(context);
    CGContextClipToRect(context, CGRectMake(x, y, w, h));
}

- (void)drawPolygon:(MPolygonGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    //set the color:
    MColorRGBA rgba = { graph->rgba() };
        
    CGFloat red   = rgba.red   / 255.f;
    CGFloat green = rgba.green / 255.f;
    CGFloat blue  = rgba.blue  / 255.f;
    CGFloat alpha = rgba.alpha / 255.f;
        
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
    
    //connect the points:
    int points = graph->points();
    for (int index = 0; index < points; ++index) {
        CGFloat x = graph->pixelX(index);
    #if TARGET_OS_OSX
        CGFloat y = viewHeight - graph->pixelY(index);
    #elif TARGET_OS_IOS
        CGFloat y = graph->pixelY(index);
    #endif
        
        if (index == 0) {
            CGContextMoveToPoint(context, x, y);
        } else {
            CGContextAddLineToPoint(context, x, y);
        }
    }
    
    //draw.
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)drawImage:(MImageGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    _NSImage *nsImage = std::static_pointer_cast<MMACImage>(graph->image())->mNSImage;
    
    CGFloat alpha = graph->alpha ();
    CGFloat w     = graph->pixelW();
    CGFloat h     = graph->pixelH();
    CGFloat x     = graph->pixelX();
    
#if TARGET_OS_OSX
    CGFloat y     = viewHeight - h - graph->pixelY();
    CGRect  dRect = CGRectMake(x, y, w, h);
    CGRect  sRect = CGRectMake(0, 0, nsImage.size.width, nsImage.size.height);
    
    NSCompositingOperation op = NSCompositingOperationSourceOver;
    [nsImage drawInRect:dRect fromRect:sRect operation:op fraction:alpha];
    
#elif TARGET_OS_IOS
    CGFloat y     = graph->pixelY();
    CGRect  dRect = CGRectMake(x, y, w, h);
    
    CGBlendMode mode = kCGBlendModeSourceAtop;
    [nsImage drawInRect:dRect blendMode:mode alpha:alpha];
    
#endif
}

- (void)drawText:(MTextGraph *)graph withContext:(CGContextRef)context viewHeight:(CGFloat)viewHeight {
    //set the text.
    NSString *text = @(graph->text().c_str());
    
    //set the color:
    MColorRGBA rgba = { graph->rgba() };
    
    CGFloat red   = rgba.red   / 255.f;
    CGFloat green = rgba.green / 255.f;
    CGFloat blue  = rgba.blue  / 255.f;
    CGFloat alpha = rgba.alpha / 255.f;
    
    _NSColor *color = [_NSColor colorWithRed:red green:green blue:blue alpha:alpha];
    
    //set the font.
    CGFloat fontSize = graph->pixelFontSize();
    _NSFont *font = [_NSFont systemFontOfSize:fontSize];
    
    //calculate the position:
    CGFloat spaceW = graph->pixelW();
    CGFloat spaceH = graph->pixelH();
    CGFloat spaceX = graph->pixelX();
#if TARGET_OS_OSX
    CGFloat spaceY = viewHeight - spaceH - graph->pixelY();
#elif TARGET_OS_IOS
    CGFloat spaceY = graph->pixelY();
#endif
    
    NSDictionary *attributes = @{
        NSForegroundColorAttributeName: color,
        NSFontAttributeName: font,
    };
    CGSize size = [text sizeWithAttributes:attributes];
        
    CGFloat x = 0;
    switch (graph->hAlign()) {
        case MHAlign::Left  : x = spaceX                             ; break;
        case MHAlign::Center: x = spaceX + (spaceW - size.width ) / 2; break;
        case MHAlign::Right : x = spaceX + (spaceW - size.width )    ; break;
    }
    
    CGFloat y = 0;
    switch (graph->vAlign()) {
    #if TARGET_OS_OSX
        case MVAlign::Top   : y = spaceY + (spaceH - size.height)    ; break;
        case MVAlign::Middle: y = spaceY + (spaceH - size.height) / 2; break;
        case MVAlign::Bottom: y = spaceY                             ; break;
    #elif TARGET_OS_IOS
        case MVAlign::Top   : y = spaceY                             ; break;
        case MVAlign::Middle: y = spaceY + (spaceH - size.height) / 2; break;
        case MVAlign::Bottom: y = spaceY + (spaceH - size.height)    ; break;
    #endif
    }
    
    //draw.
    [text drawAtPoint:CGPointMake(x, y) withAttributes:attributes];
}

@end
