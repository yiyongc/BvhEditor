#pragma once

//#include "trastercm.h"
#include "texception.h"

//#undef DVAPI
//#undef DVVAR
//#ifdef TROP_EXPORTS
//#define DVAPI DV_EXPORT_API
//#define DVVAR DV_EXPORT_VAR
//#else
//#define DVAPI DV_IMPORT_API
//#define DVVAR DV_IMPORT_VAR
//#endif

#include "traster.h"
//===============================================================

//  Forward declarations

//class TPalette;
//typedef TSmartPointerT<TPalette> TPaletteP;
//
//extern "C" {
//struct _RASTER;
//}
//
////===============================================================
//
////********************************************************************************
////    TRopException  definition
////********************************************************************************
//
class TRopException : public TException
{
	std::string message;

public:
	TRopException(const std::string &s) : message(s) {}
	~TRopException() {}

	TString getMessage() const;
};
//
////********************************************************************************
////    TRop  namespace
////********************************************************************************
//
////! The TRop namespace provides various algorithms to operate with Toonz's raster types
namespace TRop
{
//
//enum ResampleFilterType {
//	None,
//	//! triangle filter
//	Triangle,
//	//! Mitchell-Netravali filter
//	Mitchell,
//	//! cubic convolution, a = .5
//	Cubic5,
//	//! cubic convolution, a = .75
//	Cubic75,
//	//! cubic convolution, a = 1
//	Cubic1,
//	//! Hann window, rad = 2
//	Hann2,
//	//! Hann window, rad = 3
//	Hann3,
//	//! Hamming window, rad = 2
//	Hamming2,
//	//! Hamming window, rad = 3
//	Hamming3,
//	//! Lanczos window, rad = 2
//	Lanczos2,
//	//! Lanczos window, rad = 3
//	Lanczos3,
//	//! Gaussian convolution
//	Gauss,
//	//! 'Quick' filter
//	ClosestPixel,
//	//! 'Quick' filter
//	Bilinear,
//
//	HowMany
//};
//
//enum ColorMask { RChan = 0x1,
//				 GChan = 0x2,
//				 BChan = 0x4,
//				 MChan = 0x8 };
//
//enum ErodilateMaskType { ED_rectangular,
//						 ED_circular };
//
////! Applies first order mappings to each of \b rin's channels.
///*! \note The input and output rasters must have the same size and pixel type.
//            Supports rout == rin. */
//void rgbmScale(TRasterP rout, TRasterP rin,
//					 const double *k, const double *a, const int *out0, const int *out1);
//
//void rgbmScale(TRasterP rout, TRasterP rin,
//					 double kr, double kg, double kb, double km,
//					 double ar = 0.0, double ag = 0.0, double ab = 0.0, double am = 0.0);
//
///*! Transforms each input channel interval into the corresponding channel interval,
//      cropping values if necessary.
//
//      Specified pointers are associated to int[5] arrays representing the 5-ple
//      (RGB, R, G, B, M) - where RGBM adjustment apply first. Then, Global apply
//      equally on RGB channels (but not on M).*/
///*! \note The input and output rasters must have the same size and pixel type.
//            Supports rout == rin. */
//void rgbmAdjust(TRasterP rout, TRasterP rin,
//					  const int *in0, const int *in1, const int *out0, const int *out1);
//
////! Make a checkoboard in \b rout
///*! Two variables \b TPixel32 describe the checkboard color, 
//      \b TDimension are checkboard dimension and \b TPoinD the position
//  \include checkBoard_ex.cpp*/
//void checkBoard(TRasterP rout, const TPixel32 &, const TPixel32 &, const TDimensionD &, const TPointD &);
//
////! Make the image over of the \b up raster on the \b dn raster and put the resulting raster in \b rout
///*! \include over_ex1.cpp*/
//void over(const TRasterP &out, const TRasterP &dn, const TRasterP &up);
////! Make the image over of the \b up raster on the \b out raster starting from \b pos
//void over(const TRasterP &out, const TRasterP &up, const TPoint &pos = TPoint());
////! Make the image over of the \b up raster on the \b out raster
//void over(const TRasterP &out, const TRasterP &up, const TAffine &aff,
//				ResampleFilterType filterType = Triangle);
////! Make the image over of the \b up raster on the \b out raster starting from \b pos.
////! Position \b pos must be already transformed using \b aff.
//void over(const TRasterP &out, const TRasterP &up, const TPoint &pos, const TAffine &aff,
//				ResampleFilterType filterType = Triangle);
////! Make the image over of the \b up raster on the \b dn raster using a \b mask and put the resulting raster in \b rout
//void over(const TRasterP &out, const TRasterP &dn, const TRasterP &up, const TRasterGR8P &mask);
//
////! Make a coloring of \b rup with \b color and put the resulting raster in \b rout
//void over(TRaster32P rout, const TRasterGR8P &rup, const TPixel32 &color);
//
//void over(TRasterP rout, const TRasterCM32P &rup, TPalette *palette,
//				const TPoint &point = TPoint(), const TAffine &aff = TAffine());
//
////! Apply the transformation \b aff to the raster \b in and put the resulting raster in \b rout
///*!
//      It can be specified a resample filter type and a blur factor.
//    \include resample_ex.cpp
//  */
//void resample(const TRasterP &out, const TRasterP &in, const TAffine &aff,
//					ResampleFilterType filterType = Triangle, double blur = 1.);

//! Like the over function, but only uses closest_pixel filter
//void quickPut(const TRasterP &out,
//					const TRasterP &up,
//					const TAffine &aff,
//					const TPixel32 &colorScale = TPixel::Black,
//					bool doPremultiply = false,
//					bool whiteTransp = false,
//					bool firstColumn = false,
//					bool doRasterDarkenBlendedView = false);

//! Like the over function but up image must be cmapped.
/*! Only uses closest_pixel filter. 
    GlobalMatte is the opacity, used for onionskinning
  */

//void quickPut(const TRasterP &out, const TRasterCM32P &up, const TPaletteP &plt,
//					const TAffine &aff, const TPixel32 &globalColorScale = TPixel::Black, bool inksOnly = false);
//
//// for trasparency check, ink check and paint check
//
//class CmappedQuickputSettings
//{
//public:
//	TPixel32 m_globalColorScale,
//		m_transpCheckPaint,
//		m_transpCheckBg,
//		m_transpCheckInk;
//
//	int m_inkIndex,
//		m_paintIndex;
//
//	bool m_inksOnly,
//		m_transparencyCheck,
//		m_blackBgCheck;
//
//	CmappedQuickputSettings()
//		: m_globalColorScale(TPixel32::Black), m_inksOnly(false), m_transparencyCheck(false), m_blackBgCheck(false), m_inkIndex(-1), m_paintIndex(-1) {}
//};
//
//void quickPut(const TRasterP &dn, const TRasterCM32P &upCM32, const TPaletteP &plt, const TAffine &aff,
//					const CmappedQuickputSettings &settings);
//
//void quickResampleColorFilter(const TRasterP &dn, const TRasterP &up, const TAffine &aff,
//									const TPaletteP &plt, UCHAR colorMask);
//
//void convolve_3_i(TRasterP rout, TRasterP rin, int dx, int dy, double conv[]);
//
//void convolve_i(TRasterP rout, TRasterP rin, int dx, int dy, double conv[], int radius);
//
//void fracmove(TRasterP rout, TRasterP rin, double dx, double dy);
//
////! Convert \b src raster in raster type \b dst
////void convert(TRasterP dst, const TRasterP &src);
//
////! Copy \b src in \b dst
//void copy(TRasterP dst, const TRasterP &src);
//
//void brush(TRaster32P ras, const TPoint &a, const TPoint &b, int radius, const TPixel32 &color);
//
////! Get \b bbox of \b ras raster
//void computeBBox(TRasterP ras, TRect &bbox);
//
////! Add background of color \b col to the \b ras raster
//void addBackground(TRasterP ras, const TPixel32 &col);
//
////! Return true if \b ras is opaque
//bool isOpaque(TRasterP ras);
//
////! Invert \b ras color in according with \b invRed, \b invGreen, \b invBlue, \b invMatte
//void invert(TRasterP ras, bool invRed = true, bool invGreen = true, bool invBlue = true, bool invMatte = false);
//
////! Add \b rdown pixels to \b rup pixels in according with the factor \b v and put the resulting raster in \b rout
//void add(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout, double v);
//
////! Add \b rdown pixels to \b rup pixels and put the resulting raster in \b rout
//void add(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void colordodge(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void colorburn(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void screen(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
////! Subtract \b rdown pixels to \b rup pixels and put the resulting raster in \b rout
//void sub(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout, bool matte);
//
//void mult(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout, int v, bool matte); //[-255..+255]
//
//void ropin(const TRasterP &source, const TRasterP &matte, const TRasterP &rout);
//
//void ropout(const TRasterP &source, const TRasterP &matte, const TRasterP &rout);
//
//void atop(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void txor(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void crossDissolve(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout, UCHAR v); //[0..255]
//
//void lighten(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void darken(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
//void ropmin(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout, bool matte);
//
//void ropmax(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//void linearburn(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//void overlay(const TRasterP &rup, const TRasterP &rdown, const TRasterP &rout);
//
////! Make a premultiply of all raster pixels
void premultiply(const TRasterP &ras);
//
////! Make a depremultiply of all raster pixels
//void depremultiply(const TRasterP &ras);
//
////! all white pixels are set to transparent
//void whiteTransp(const TRasterP &ras);
//
//TRasterP shrink(TRasterP rin, int shrink);
//
////! Make a \b gamma correct in the \b raster
//void gammaCorrect(TRasterP raster, double gamma);
//
////! Make a \b gamma correct in the \b raster
//void gammaCorrectRGBM(TRasterP raster, double gammar, double gammag, double gammab, double gammam);
//
////! Return the pixel size of the \b blur border
//int getBlurBorder(double blur);
//
////! Inserts antialias around jaggy lines. Threshold is a pixel distance intended from 0 to 256.
////! Softness may vary from 0 to 100.
//void antialias(const TRasterP &src, const TRasterP &dst, int threshold, int softness);
//
////! Performs despeckling by replacing the color of small uniform areas (those enclosable in a
////! square of specified edge length) with that most occurring along the area border.
//void majorityDespeckle(const TRasterP &ras, int sizeThreshold);

//!Make blur effect on \b srcRas raster and put the result in \b dstRas.
void blur(
	const TRasterP &dstRas,
	const TRasterP &srcRas,
	double blur,
	int dx, int dy,
	bool useSSE = false);
//
//struct RaylitParams {
//	TPixel m_color;
//	T3DPoint m_lightOriginSrc;
//	T3DPoint m_lightOriginDst;
//	double m_smoothness;
//	double m_decay;
//	double m_intensity;
//	double m_scale;
//	bool m_invert;
//	bool m_includeInput;
//};
//
////! Make raylit effect on \b srcRas raster and put the result in \b dstRas.
//void raylit(const TRasterP &dstRas, const TRasterP &srcRas, const RaylitParams &params);
//void glassRaylit(const TRasterP &dstRas, const TRasterP &srcRas, const RaylitParams &params);
//
//void despeckle(const TRasterP &ras, int sizeThreshold, bool check, bool trasparentIsWhite = false);
//void despeckle(const TRasterP &rout, const TRasterP &rin, int sizeThreshold, bool check);
//
////! Return a raster with r b channels swapped
//TRaster32P copyAndSwapRBChannels(const TRaster32P &srcRaster);
//
////result in 'left'
//void makeStereoRaster(const TRasterP &left, const TRasterP &right);
//
//void setChannel(const TRasterP &rin, TRasterP rout, UCHAR chan, bool greytones);
//
///*!
//    Applies classical morphological erosion/dilation to the specified input raster.
//    \note Supports rin == rout.
//  */
//void erodilate(const TRasterP &rin, const TRasterP &rout,
//					 double radius, ErodilateMaskType type);
//
//#ifdef TNZ_MACHINE_CHANNEL_ORDER_MRGB
//void swapRBChannels(const TRaster32P &r);
//#endif

///////////////////////////////////////
//  Utilities for Toonz 4.6 porting  //
///////////////////////////////////////

//! Convert TRasterP in an old toonz raster!
/*! Use the same buffer, not creates a new raster (the palette is new instead!)
  */
//_RASTER *convertRaster50to46(const TRasterP &inRas, const TPaletteP &inPalette);
//
////! Release the old toonz raster.
///*! If the \b doReleaseBuffer is true, the buffer is released
//    (the palette is always deleted!)
//  */
//void releaseRaster46(_RASTER *&r, bool doReleaseBuffer = false);
//
////! Read an image in the path \b filename and put it in an old toonz raster
//_RASTER *readRaster46(const char *filename);
//
////! Retrieves the input raster from Toonz internal cache. Need to call it before using the raster.
//void lockRaster(_RASTER *raster);
//
////! Surrenders the input raster's buffer to the cache. Should be called before inactivity periods.
//void unlockRaster(_RASTER *raster);

} // TRop namespace
