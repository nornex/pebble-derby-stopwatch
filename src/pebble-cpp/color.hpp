//
// Created by matt on 27/11/15.
//

#ifndef PEBBLE_CPP_COLOR_HPP
#define PEBBLE_CPP_COLOR_HPP

#include "pebble-sdk.hpp"

namespace pebble
{
    class Color
    {
    public:

        #ifdef PBL_BW
            typedef GColor TColor;
        #else
            typedef GColor8 TColor;
        #endif

        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        {
            #ifdef PBL_BW
                if (alpha)
                {
                    color_ = red || green || blue ? GColorBlack : GColorWhite;
                }
                else
                {
                    color_ = GColorClear;
                }
            #else
                color_.argb =
                    ((alpha >> 6)   << 6) |
                    ((red >> 6)     << 4) |
                    ((green >> 6)   << 2) |
                    ((blue >> 6)    << 0);
            #endif
        }

        Color(uint8_t argb)
        {
            #ifdef PBL_BW
                if (argb & 0b11000000)
                {
                    color_ =  (argb & 0b00111111) ? GColorBlack : GColorWhite;
                }
                else
                {
                    color_ = GColorClear;
                }
            #else
                color_.argb = argb;
            #endif
        }

        Color(TColor other)
        {
            memcpy(&color_, &other, sizeof(TColor));
        }

        uint8_t red() const
        {
            #ifdef PBL_BW
                return color_ == GColorBlack ? 0 : 255;
            #else
                return color_.r << 6;
            #endif
        }

        uint8_t green() const
        {
            #ifdef PBL_BW
                return color_ == GColorBlack ? 0 : 255;
            #else
                return color_.g << 6;
            #endif
        }

        uint8_t blue() const
        {
            #ifdef PBL_BW
                return color_ == GColorBlack ? 0 : 255;
            #else
                return color_.b << 6;
            #endif
        }

        uint8_t alpha() const
        {
            #ifdef PBL_BW
                return color_ == GColorClear ? 0 : 255;
            #else
                return color_.a << 6;
            #endif
        }

        uint8_t argb() const
        {
            #ifdef PBL_BW
                return
                    color_ == GColorBlack ? uint8_t(0b11000000) :
                    color_ == GColorWhite ? uint8_t(0b11111111) :
                    uint8_t(0b00111111);
            #else
                return color_.argb;
            #endif
        }

        Color WithAlpha(uint8_t alpha) const
        {
            #ifdef PBL_BW
                return alpha ? (color_ == GColorClear ? GColorBlack : color_) : GColorClear;
            #else
                Color altered(color_);
                altered.color_.a = alpha >> 6;
                return altered;
            #endif
        }

        TColor sdk_value() const { return color_; }

        // Color definitions

        static Color Black();
        static Color OxfordBlue();
        static Color DukeBlue();
        static Color Blue();
        static Color DarkGreen();
        static Color MidnightGreen();
        static Color CobaltBlue();
        static Color BlueMoon();
        static Color IslamicGreen();
        static Color JaegerGreen();
        static Color TiffanyBlue();
        static Color VividCerulean();
        static Color Green();
        static Color Malachite();
        static Color MediumSpringGreen();
        static Color Cyan();
        static Color BulgarianRose();
        static Color ImperialPurple();
        static Color Indigo();
        static Color ElectricUltramarine();
        static Color ArmyGreen();
        static Color DarkGray();
        static Color Liberty();
        static Color VeryLightBlue();
        static Color KellyGreen();
        static Color MayGreen();
        static Color CadetBlue();
        static Color PictonBlue();
        static Color BrightGreen();
        static Color ScreaminGreen();
        static Color MediumAquamarine();
        static Color ElectricBlue();
        static Color DarkCandyAppleRed();
        static Color JazzberryJam();
        static Color Purple();
        static Color VividViolet();
        static Color WindsorTan();
        static Color RoseVale();
        static Color Purpureus();
        static Color LavenderIndigo();
        static Color Limerick();
        static Color Brass();
        static Color LightGray();
        static Color BabyBlueEyes();
        static Color SpringBud();
        static Color Inchworm();
        static Color MintGreen();
        static Color Celeste();
        static Color Red();
        static Color Folly();
        static Color FashionMagenta();
        static Color Magenta();
        static Color Orange();
        static Color SunsetOrange();
        static Color BrilliantRose();
        static Color ShockingPink();
        static Color ChromeYellow();
        static Color Rajah();
        static Color Melon();
        static Color RichBrilliantLavender();
        static Color Yellow();
        static Color Icterine();
        static Color PastelYellow();
        static Color White();
        static Color Clear();

    private:
        TColor color_;
    };

    inline Color Color::Black() { return PBL_IF_COLOR_ELSE(GColorBlackARGB8, GColorBlack); }
    inline Color Color::OxfordBlue() { return PBL_IF_COLOR_ELSE(GColorOxfordBlueARGB8, GColorBlack); }
    inline Color Color::DukeBlue() { return PBL_IF_COLOR_ELSE(GColorDukeBlueARGB8, GColorBlack); }
    inline Color Color::Blue() { return PBL_IF_COLOR_ELSE(GColorBlueARGB8, GColorBlack); }
    inline Color Color::DarkGreen() { return PBL_IF_COLOR_ELSE(GColorDarkGreenARGB8, GColorBlack); }
    inline Color Color::MidnightGreen() { return PBL_IF_COLOR_ELSE(GColorMidnightGreenARGB8, GColorBlack); }
    inline Color Color::CobaltBlue() { return PBL_IF_COLOR_ELSE(GColorCobaltBlueARGB8, GColorBlack); }
    inline Color Color::BlueMoon() { return PBL_IF_COLOR_ELSE(GColorBlueMoonARGB8, GColorBlack); }
    inline Color Color::IslamicGreen() { return PBL_IF_COLOR_ELSE(GColorIslamicGreenARGB8, GColorBlack); }
    inline Color Color::JaegerGreen() { return PBL_IF_COLOR_ELSE(GColorJaegerGreenARGB8, GColorBlack); }
    inline Color Color::TiffanyBlue() { return PBL_IF_COLOR_ELSE(GColorTiffanyBlueARGB8, GColorBlack); }
    inline Color Color::VividCerulean() { return PBL_IF_COLOR_ELSE(GColorVividCeruleanARGB8, GColorBlack); }
    inline Color Color::Green() { return PBL_IF_COLOR_ELSE(GColorGreenARGB8, GColorBlack); }
    inline Color Color::Malachite() { return PBL_IF_COLOR_ELSE(GColorMalachiteARGB8, GColorBlack); }
    inline Color Color::MediumSpringGreen() { return PBL_IF_COLOR_ELSE(GColorMediumSpringGreenARGB8, GColorBlack); }
    inline Color Color::Cyan() { return PBL_IF_COLOR_ELSE(GColorCyanARGB8, GColorBlack); }
    inline Color Color::BulgarianRose() { return PBL_IF_COLOR_ELSE(GColorBulgarianRoseARGB8, GColorBlack); }
    inline Color Color::ImperialPurple() { return PBL_IF_COLOR_ELSE(GColorImperialPurpleARGB8, GColorBlack); }
    inline Color Color::Indigo() { return PBL_IF_COLOR_ELSE(GColorIndigoARGB8, GColorBlack); }
    inline Color Color::ElectricUltramarine() { return PBL_IF_COLOR_ELSE(GColorElectricUltramarineARGB8, GColorBlack); }
    inline Color Color::ArmyGreen() { return PBL_IF_COLOR_ELSE(GColorArmyGreenARGB8, GColorBlack); }
    inline Color Color::DarkGray() { return PBL_IF_COLOR_ELSE(GColorDarkGrayARGB8, GColorBlack); }
    inline Color Color::Liberty() { return PBL_IF_COLOR_ELSE(GColorLibertyARGB8, GColorBlack); }
    inline Color Color::VeryLightBlue() { return PBL_IF_COLOR_ELSE(GColorVeryLightBlueARGB8, GColorBlack); }
    inline Color Color::KellyGreen() { return PBL_IF_COLOR_ELSE(GColorKellyGreenARGB8, GColorBlack); }
    inline Color Color::MayGreen() { return PBL_IF_COLOR_ELSE(GColorMayGreenARGB8, GColorBlack); }
    inline Color Color::CadetBlue() { return PBL_IF_COLOR_ELSE(GColorCadetBlueARGB8, GColorBlack); }
    inline Color Color::PictonBlue() { return PBL_IF_COLOR_ELSE(GColorPictonBlueARGB8, GColorBlack); }
    inline Color Color::BrightGreen() { return PBL_IF_COLOR_ELSE(GColorBrightGreenARGB8, GColorBlack); }
    inline Color Color::ScreaminGreen() { return PBL_IF_COLOR_ELSE(GColorScreaminGreenARGB8, GColorBlack); }
    inline Color Color::MediumAquamarine() { return PBL_IF_COLOR_ELSE(GColorMediumAquamarineARGB8, GColorBlack); }
    inline Color Color::ElectricBlue() { return PBL_IF_COLOR_ELSE(GColorElectricBlueARGB8, GColorBlack); }
    inline Color Color::DarkCandyAppleRed() { return PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRedARGB8, GColorBlack); }
    inline Color Color::JazzberryJam() { return PBL_IF_COLOR_ELSE(GColorJazzberryJamARGB8, GColorBlack); }
    inline Color Color::Purple() { return PBL_IF_COLOR_ELSE(GColorPurpleARGB8, GColorBlack); }
    inline Color Color::VividViolet() { return PBL_IF_COLOR_ELSE(GColorVividVioletARGB8, GColorBlack); }
    inline Color Color::WindsorTan() { return PBL_IF_COLOR_ELSE(GColorWindsorTanARGB8, GColorBlack); }
    inline Color Color::RoseVale() { return PBL_IF_COLOR_ELSE(GColorRoseValeARGB8, GColorBlack); }
    inline Color Color::Purpureus() { return PBL_IF_COLOR_ELSE(GColorPurpureusARGB8, GColorBlack); }
    inline Color Color::LavenderIndigo() { return PBL_IF_COLOR_ELSE(GColorLavenderIndigoARGB8, GColorBlack); }
    inline Color Color::Limerick() { return PBL_IF_COLOR_ELSE(GColorLimerickARGB8, GColorBlack); }
    inline Color Color::Brass() { return PBL_IF_COLOR_ELSE(GColorBrassARGB8, GColorBlack); }
    inline Color Color::LightGray() { return PBL_IF_COLOR_ELSE(GColorLightGrayARGB8, GColorBlack); }
    inline Color Color::BabyBlueEyes() { return PBL_IF_COLOR_ELSE(GColorBabyBlueEyesARGB8, GColorBlack); }
    inline Color Color::SpringBud() { return PBL_IF_COLOR_ELSE(GColorSpringBudARGB8, GColorBlack); }
    inline Color Color::Inchworm() { return PBL_IF_COLOR_ELSE(GColorInchwormARGB8, GColorBlack); }
    inline Color Color::MintGreen() { return PBL_IF_COLOR_ELSE(GColorMintGreenARGB8, GColorBlack); }
    inline Color Color::Celeste() { return PBL_IF_COLOR_ELSE(GColorCelesteARGB8, GColorBlack); }
    inline Color Color::Red() { return PBL_IF_COLOR_ELSE(GColorRedARGB8, GColorBlack); }
    inline Color Color::Folly() { return PBL_IF_COLOR_ELSE(GColorFollyARGB8, GColorBlack); }
    inline Color Color::FashionMagenta() { return PBL_IF_COLOR_ELSE(GColorFashionMagentaARGB8, GColorBlack); }
    inline Color Color::Magenta() { return PBL_IF_COLOR_ELSE(GColorMagentaARGB8, GColorBlack); }
    inline Color Color::Orange() { return PBL_IF_COLOR_ELSE(GColorOrangeARGB8, GColorBlack); }
    inline Color Color::SunsetOrange() { return PBL_IF_COLOR_ELSE(GColorSunsetOrangeARGB8, GColorBlack); }
    inline Color Color::BrilliantRose() { return PBL_IF_COLOR_ELSE(GColorBrilliantRoseARGB8, GColorBlack); }
    inline Color Color::ShockingPink() { return PBL_IF_COLOR_ELSE(GColorShockingPinkARGB8, GColorBlack); }
    inline Color Color::ChromeYellow() { return PBL_IF_COLOR_ELSE(GColorChromeYellowARGB8, GColorBlack); }
    inline Color Color::Rajah() { return PBL_IF_COLOR_ELSE(GColorRajahARGB8, GColorBlack); }
    inline Color Color::Melon() { return PBL_IF_COLOR_ELSE(GColorMelonARGB8, GColorBlack); }
    inline Color Color::RichBrilliantLavender() { return PBL_IF_COLOR_ELSE(GColorRichBrilliantLavenderARGB8, GColorBlack); }
    inline Color Color::Yellow() { return PBL_IF_COLOR_ELSE(GColorYellowARGB8, GColorBlack); }
    inline Color Color::Icterine() { return PBL_IF_COLOR_ELSE(GColorIcterineARGB8, GColorBlack); }
    inline Color Color::PastelYellow() { return PBL_IF_COLOR_ELSE(GColorPastelYellowARGB8, GColorBlack); }
    inline Color Color::White() { return PBL_IF_COLOR_ELSE(GColorWhiteARGB8, GColorWhite); }
    inline Color Color::Clear() { return PBL_IF_COLOR_ELSE(GColorClearARGB8, GColorClear); }
}

#endif //PEBBLE_CPP_COLOR_HPP
