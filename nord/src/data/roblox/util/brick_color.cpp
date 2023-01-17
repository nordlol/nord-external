#include "brick_color.hpp"

#include <map>
#include <vector>

namespace nord::rbx
{
    class brick_color::brick_map
    {
       public:
        std::vector< brick_color > all_colors;

        struct color_info
        {
            bool valid;
            bool deprecated;
            ImColor color;
            std::string name;

            color_info() : valid( false ), deprecated( true )
            {
            }
        };

        std::vector< color_info > map;

        std::map< brick_color::brick_number, int > palette_map;
        std::map< brick_color::brick_number, int > rendering_palette_map;

        static brick_map& get();

       private:
        void insert( brick_color::brick_number number, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::string name )
        {
            all_colors.push_back( brick_color( number ) );

            if ( map.size() <= static_cast< unsigned int >( number ) )
                map.resize( static_cast< std::int32_t >( number ) + 1 );

            color_info& info = map[ static_cast< std::int32_t >( number ) ];

            info.valid = true;
            info.color = ImColor{ r, g, b };
            info.name = name;
        }

        brick_map()
        {
            // note all color "green" values changed to avoid trademark issues
            // public names also changed
            insert( brick_color::brick_number::brick_1, 242, 243, 243, "White" );                       //	CoolGrey 1 C
            insert( brick_color::brick_number::brick_2, 161, 165, 162, "Grey" );                        //	422 C
            insert( brick_color::brick_number::brick_3, 249, 233, 153, "Light yellow" );                //	1215 C
            insert( brick_color::brick_number::brick_5, 215, 197, 154, "Brick yellow" );                //	467 C
            insert( brick_color::brick_number::brick_6, 194, 218, 184, "Light green (Mint)" );          //	351 C
            insert( brick_color::brick_number::brick_9, 232, 186, 200, "Light reddish violet" );        //	203 C
            insert( brick_color::brick_number::brick_11, 0x80, 0xbb, 0xdb, "Pastel Blue" );             //
            insert( brick_color::brick_number::brick_12, 203, 132, 66, "Light orange brown" );          //	1385 C
            insert( brick_color::brick_number::brick_18, 204, 142, 105, "Nougat" );                     //	472 C
            insert( brick_color::brick_number::brick_21, 196, 40, 28, "Bright red" );                   //	032 C
            insert( brick_color::brick_number::brick_22, 196, 112, 160, "Med. reddish violet" );        //	2375 C
            insert( brick_color::brick_number::brick_23, 13, 105, 172, "Bright blue" );                 //	293 C
            insert( brick_color::brick_number::brick_24, 245, 205, 48, "Bright yellow" );               //	116 C
            insert( brick_color::brick_number::brick_25, 98, 71, 50, "Earth orange" );                  //	469 C
            insert( brick_color::brick_number::brick_26, 27, 42, 53, "Black" );                         //	Process Black C
            insert( brick_color::brick_number::brick_27, 109, 110, 108, "Dark grey" );                  //	418 C
            insert( brick_color::brick_number::brick_28, 40, 127, 71, "Dark green" );                   //	348 C
            insert( brick_color::brick_number::brick_29, 161, 196, 140, "Medium green" );               //	353 C
            insert( brick_color::brick_number::brick_36, 243, 207, 155, "Lig. Yellowich orange" );      //	148 C
            insert( brick_color::brick_number::brick_37, 75, 151, 75, "Bright green" );                 //	355 C
            insert( brick_color::brick_number::brick_38, 160, 95, 53, "Dark orange" );                  //	471 C
            insert( brick_color::brick_number::brick_39, 193, 202, 222, "Light bluish violet" );        //	2706 C
            insert( brick_color::brick_number::brick_40, 236, 236, 236, "Transparent" );                //	CoolGrey 1 C
            insert( brick_color::brick_number::brick_41, 205, 84, 75, "Tr. Red" );                      //	185 C
            insert( brick_color::brick_number::brick_42, 193, 223, 240, "Tr. Lg blue" );                //	304 C
            insert( brick_color::brick_number::brick_43, 123, 182, 232, "Tr. Blue" );                   //	298 C
            insert( brick_color::brick_number::brick_44, 247, 241, 141, "Tr. Yellow" );                 //	393 C
            insert( brick_color::brick_number::brick_45, 180, 210, 228, "Light blue" );                 //	545 C
            insert( brick_color::brick_number::brick_47, 217, 133, 108, "Tr. Flu. Reddish orange" );    //	165 C
            insert( brick_color::brick_number::brick_48, 132, 182, 141, "Tr. Green" );                  //	360 C
            insert( brick_color::brick_number::brick_49, 248, 241, 132, "Tr. Flu. Green" );             //	387 C
            insert( brick_color::brick_number::brick_50, 236, 232, 222, "Phosph. White" );              //	427 C
            insert( brick_color::brick_number::brick_100, 238, 196, 182, "Light red" );                 //	169 C
            insert( brick_color::brick_number::brick_101, 218, 134, 122, "Medium red" );                //	170 C
            insert( brick_color::brick_number::brick_102, 110, 153, 202, "Medium blue" );               //	284 C
            insert( brick_color::brick_number::brick_103, 199, 193, 183, "Light grey" );                //	421 C
            insert( brick_color::brick_number::brick_104, 107, 50, 124, "Bright violet" );              //	2592 C
            insert( brick_color::brick_number::brick_105, 226, 155, 64, "Br. yellowish orange" );       //	137 C
            insert( brick_color::brick_number::brick_106, 218, 133, 65, "Bright orange" );              //	151 C
            insert( brick_color::brick_number::brick_107, 0, 143, 156, "Bright bluish green" );         //	327 C
            insert( brick_color::brick_number::brick_108, 104, 92, 67, "Earth yellow" );                //	1405 C
            insert( brick_color::brick_number::brick_110, 67, 84, 147, "Bright bluish violet" );        //	2736 C
            insert( brick_color::brick_number::brick_111, 191, 183, 177, "Tr. Brown" );                 //	WarmGrey 3 C
            insert( brick_color::brick_number::brick_112, 104, 116, 172, "Medium bluish violet" );      //	2726 C
            insert( brick_color::brick_number::brick_113, 228, 173, 200, "Tr. Medi. reddish violet" );  //	230 C
            insert( brick_color::brick_number::brick_115, 199, 210, 60, "Med. yellowish green" );       //	381 C
            insert( brick_color::brick_number::brick_116, 85, 165, 175, "Med. bluish green" );          //	326 C
            insert( brick_color::brick_number::brick_118, 183, 215, 213, "Light bluish green" );        //	324 C
            insert( brick_color::brick_number::brick_119, 164, 189, 71, "Br. yellowish green" );        //	390 C
            insert( brick_color::brick_number::brick_120, 217, 228, 167, "Lig. yellowish green" );      //	365 C
            insert( brick_color::brick_number::brick_121, 231, 172, 88, "Med. yellowish orange" );      //	1365 C
            insert( brick_color::brick_number::brick_123, 211, 111, 76, "Br. reddish orange" );         //	165 C
            insert( brick_color::brick_number::brick_124, 146, 57, 120, "Bright reddish violet" );      //	241 C
            insert( brick_color::brick_number::brick_125, 234, 184, 146, "Light orange" );              //	1555 C
            insert( brick_color::brick_number::brick_126, 165, 165, 203, "Tr. Bright bluish violet" );  //	271 C
            insert( brick_color::brick_number::brick_127, 220, 188, 129, "Gold" );                      //	156 C
            insert( brick_color::brick_number::brick_128, 174, 122, 89, "Dark nougat" );                //	471 C
            insert( brick_color::brick_number::brick_131, 156, 163, 168, "Silver" );                    //	429 C
            insert( brick_color::brick_number::brick_133, 213, 115, 61, "Neon orange" );                //	165 C
            insert( brick_color::brick_number::brick_134, 216, 221, 86, "Neon green" );                 //	374 C
            insert( brick_color::brick_number::brick_135, 116, 134, 157, "Sand blue" );                 //	5415 C
            insert( brick_color::brick_number::brick_136, 135, 124, 144, "Sand violet" );               //	666 C
            insert( brick_color::brick_number::brick_137, 224, 152, 100, "Medium orange" );             //	1575 C
            insert( brick_color::brick_number::brick_138, 149, 138, 115, "Sand yellow" );               //	451 C
            insert( brick_color::brick_number::brick_140, 32, 58, 86, "Earth blue" );                   //	2955 C
            insert( brick_color::brick_number::brick_141, 39, 70, 45, "Earth green" );                  //	350 C
            insert( brick_color::brick_number::brick_143, 207, 226, 247, "Tr. Flu. Blue" );             //	657 C
            insert( brick_color::brick_number::brick_145, 121, 136, 161, "Sand blue metallic" );        //	652C
            insert( brick_color::brick_number::brick_146, 149, 142, 163, "Sand violet metallic" );      //	5285 C
            insert( brick_color::brick_number::brick_147, 147, 135, 103, "Sand yellow metallic" );      //	873 C
            insert( brick_color::brick_number::brick_148, 87, 88, 87, "Dark grey metallic" );           //	446 C
            insert( brick_color::brick_number::brick_149, 22, 29, 50, "Black metallic" );               //	2767 C
            insert( brick_color::brick_number::brick_150, 171, 173, 172, "Light grey metallic" );       //	422 C
            insert( brick_color::brick_number::brick_151, 120, 144, 130, "Sand green" );                //	624 C
            insert( brick_color::brick_number::brick_153, 149, 121, 119, "Sand red" );                  //	4995 C
            insert( brick_color::brick_number::brick_154, 123, 46, 47, "Dark red" );                    //	194 C
            insert( brick_color::brick_number::brick_157, 255, 246, 123, "Tr. Flu. Yellow" );           //	395 C
            insert( brick_color::brick_number::brick_158, 225, 164, 194, "Tr. Flu. Red" );              //	211 C
            insert( brick_color::brick_number::brick_168, 117, 108, 98, "Gun metallic" );               //	WarmGrey 11 C
            insert( brick_color::brick_number::brick_176, 151, 105, 91, "Red flip/flop" );              //	483 C
            insert( brick_color::brick_number::brick_178, 180, 132, 85, "Yellow flip/flop" );           //	160 C
            insert( brick_color::brick_number::brick_179, 137, 135, 136, "Silver flip/flop" );          //	410 C
            insert( brick_color::brick_number::brick_180, 215, 169, 75, "Curry" );                      //	131 C
            insert( brick_color::brick_number::brick_190, 249, 214, 46, "Fire Yellow" );                //	012 C
            insert( brick_color::brick_number::brick_191, 232, 171, 45, "Flame yellowish orange" );     //	137 C
            insert( brick_color::brick_number::brick_192, 105, 64, 40, "Reddish brown" );               //	499 C
            insert( brick_color::brick_number::brick_193, 207, 96, 36, "Flame reddish orange" );        //	173 C
            insert( brick_color::brick_number::brick_194, 163, 162, 165, "Medium stone grey" );         //	429 C
            insert( brick_color::brick_number::brick_195, 70, 103, 164, "Royal blue" );                 //	2728 C
            insert( brick_color::brick_number::brick_196, 35, 71, 139, "Dark Royal blue" );             //	286 C
            insert( brick_color::brick_number::brick_198, 142, 66, 133, "Bright reddish lilac" );       //	254 C
            insert( brick_color::brick_number::brick_199, 99, 95, 98, "Dark stone grey" );              //	431 C
            insert( brick_color::brick_number::brick_200, 130, 138, 93, "Lemon metalic" );              //	5767 C
            insert( brick_color::brick_number::brick_208, 229, 228, 223, "Light stone grey" );          //	428 C
            insert( brick_color::brick_number::brick_209, 176, 142, 68, "Dark Curry" );                 //	118 C
            insert( brick_color::brick_number::brick_210, 112, 149, 120, "Faded green" );               //	364 C
            insert( brick_color::brick_number::brick_211, 121, 181, 181, "Turquoise" );                 //	3255 C
            insert( brick_color::brick_number::brick_212, 159, 195, 233, "Light Royal blue" );          //	292 C
            insert( brick_color::brick_number::brick_213, 108, 129, 183, "Medium Royal blue" );         //	2727 C
            insert( brick_color::brick_number::brick_216, 143, 76, 42, "Rust" );                        //	174 C
            insert( brick_color::brick_number::brick_217, 124, 92, 70, "Brown" );                       //	161 C
            insert( brick_color::brick_number::brick_218, 150, 112, 159, "Reddish lilac" );             //	2573 C
            insert( brick_color::brick_number::brick_219, 107, 98, 155, "Lilac" );                      //	2725 C
            insert( brick_color::brick_number::brick_220, 167, 169, 206, "Light lilac" );               //	2716 C
            insert( brick_color::brick_number::brick_221, 205, 98, 152, "Bright purple" );              //	232 C
            insert( brick_color::brick_number::brick_222, 228, 173, 200, "Light purple" );              //	236 C
            insert( brick_color::brick_number::brick_223, 220, 144, 149, "Light pink" );                //	183 C
            insert( brick_color::brick_number::brick_224, 240, 213, 160, "Light brick yellow" );        //	7501 C
            insert( brick_color::brick_number::brick_225, 235, 184, 127, "Warm yellowish orange" );     //	713 C
            insert( brick_color::brick_number::brick_226, 253, 234, 141, "Cool yellow" );               //	120 C
            insert( brick_color::brick_number::brick_232, 125, 187, 221, "Dove blue" );                 //	311 C
            insert( brick_color::brick_number::brick_268, 52, 43, 117, "Medium lilac" );                //	2685 C

            insert( brick_color::brick_number::brick_301, 80, 109, 84, "Slime green" );
            insert( brick_color::brick_number::brick_302, 91, 93, 105, "Smoky grey" );
            insert( brick_color::brick_number::brick_303, 0, 16, 176, "Dark blue" );
            insert( brick_color::brick_number::brick_304, 44, 101, 29, "Parsley green" );
            insert( brick_color::brick_number::brick_305, 82, 124, 174, "Steel blue" );
            insert( brick_color::brick_number::brick_306, 51, 88, 130, "Storm blue" );
            insert( brick_color::brick_number::brick_307, 16, 42, 220, "Lapis" );
            insert( brick_color::brick_number::brick_308, 61, 21, 133, "Dark indigo" );
            insert( brick_color::brick_number::brick_309, 52, 142, 64, "Sea green" );
            insert( brick_color::brick_number::brick_310, 91, 154, 76, "Shamrock" );
            insert( brick_color::brick_number::brick_311, 159, 161, 172, "Fossil" );
            insert( brick_color::brick_number::brick_312, 89, 34, 89, "Mulberry" );
            insert( brick_color::brick_number::brick_313, 31, 128, 29, "Forest green" );
            insert( brick_color::brick_number::brick_314, 159, 173, 192, "Cadet blue" );
            insert( brick_color::brick_number::brick_315, 9, 137, 207, "Electric blue" );
            insert( brick_color::brick_number::brick_316, 123, 0, 123, "Eggplant" );
            insert( brick_color::brick_number::brick_317, 124, 156, 107, "Moss" );
            insert( brick_color::brick_number::brick_318, 138, 171, 133, "Artichoke" );
            insert( brick_color::brick_number::brick_319, 185, 196, 177, "Sage green" );
            insert( brick_color::brick_number::brick_320, 202, 203, 209, "Ghost grey" );
            insert( brick_color::brick_number::brick_321, 167, 94, 155, "Lilac" );
            insert( brick_color::brick_number::brick_322, 123, 47, 123, "Plum" );
            insert( brick_color::brick_number::brick_323, 148, 190, 129, "Olivine" );
            insert( brick_color::brick_number::brick_324, 168, 189, 153, "Laurel green" );
            insert( brick_color::brick_number::brick_325, 223, 223, 222, "Quill grey" );
            insert( brick_color::brick_number::brick_326, 218, 220, 225, "Iron" );
            insert( brick_color::brick_number::brick_327, 151, 0, 0, "Crimson" );
            insert( brick_color::brick_number::brick_328, 177, 229, 166, "Mint" );
            insert( brick_color::brick_number::brick_329, 152, 194, 219, "Baby blue" );
            insert( brick_color::brick_number::brick_330, 255, 152, 220, "Carnation pink" );
            insert( brick_color::brick_number::brick_331, 255, 89, 89, "Persimmon" );
            insert( brick_color::brick_number::brick_332, 117, 0, 0, "Maroon" );
            insert( brick_color::brick_number::brick_333, 239, 184, 56, "Gold" );
            insert( brick_color::brick_number::brick_334, 248, 217, 109, "Daisy orange" );
            insert( brick_color::brick_number::brick_335, 231, 231, 236, "Pearl" );
            insert( brick_color::brick_number::brick_336, 199, 212, 228, "Fog" );
            insert( brick_color::brick_number::brick_337, 255, 148, 148, "Salmon" );
            insert( brick_color::brick_number::brick_338, 190, 104, 98, "Terra Cotta" );
            insert( brick_color::brick_number::brick_339, 86, 36, 36, "Cocoa" );
            insert( brick_color::brick_number::brick_340, 241, 231, 199, "Wheat" );
            insert( brick_color::brick_number::brick_341, 254, 243, 187, "Buttermilk" );
            insert( brick_color::brick_number::brick_342, 224, 178, 208, "Mauve" );
            insert( brick_color::brick_number::brick_343, 212, 144, 189, "Sunrise" );
            insert( brick_color::brick_number::brick_344, 150, 85, 85, "Tawny" );
            insert( brick_color::brick_number::brick_345, 143, 76, 42, "Rust" );
            insert( brick_color::brick_number::brick_346, 211, 190, 150, "Cashmere" );
            insert( brick_color::brick_number::brick_347, 226, 220, 188, "Khaki" );
            insert( brick_color::brick_number::brick_348, 237, 234, 234, "Lily white" );
            insert( brick_color::brick_number::brick_349, 233, 218, 218, "Seashell" );
            insert( brick_color::brick_number::brick_350, 136, 62, 62, "Burgundy" );
            insert( brick_color::brick_number::brick_351, 188, 155, 93, "Cork" );
            insert( brick_color::brick_number::brick_352, 199, 172, 120, "Burlap" );
            insert( brick_color::brick_number::brick_353, 202, 191, 163, "Beige" );
            insert( brick_color::brick_number::brick_354, 187, 179, 178, "Oyster" );
            insert( brick_color::brick_number::brick_355, 108, 88, 75, "Pine Cone" );
            insert( brick_color::brick_number::brick_356, 160, 132, 79, "Fawn brown" );
            insert( brick_color::brick_number::brick_357, 149, 137, 136, "Hurricane grey" );
            insert( brick_color::brick_number::brick_358, 171, 168, 158, "Cloudy grey" );
            insert( brick_color::brick_number::brick_359, 175, 148, 131, "Linen" );
            insert( brick_color::brick_number::brick_360, 150, 103, 102, "Copper" );
            insert( brick_color::brick_number::brick_361, 86, 66, 54, "Dirt brown" );
            insert( brick_color::brick_number::brick_362, 126, 104, 63, "Bronze" );
            insert( brick_color::brick_number::brick_363, 105, 102, 92, "Flint" );
            insert( brick_color::brick_number::brick_364, 90, 76, 66, "Dark taupe" );
            insert( brick_color::brick_number::brick_365, 106, 57, 9, "Burnt Sienna" );

            insert( brick_color::brick_number::roblox_1001, 248, 248, 248, "Institutional white" );
            insert( brick_color::brick_number::roblox_1002, 205, 205, 205, "Mid gray" );
            insert( brick_color::brick_number::roblox_1003, 17, 17, 17, "Really black" );
            insert( brick_color::brick_number::roblox_1004, 255, 0, 0, "Really red" );
            insert( brick_color::brick_number::roblox_1005, 255, 175, 0, "Deep orange" );
            insert( brick_color::brick_number::roblox_1006, 180, 128, 255, "Alder" );
            insert( brick_color::brick_number::roblox_1007, 163, 75, 75, "Dusty Rose" );
            insert( brick_color::brick_number::roblox_1008, 193, 190, 66, "Olive" );
            insert( brick_color::brick_number::roblox_1009, 255, 255, 0, "New Yeller" );
            insert( brick_color::brick_number::roblox_1010, 0, 0, 255, "Really blue" );
            insert( brick_color::brick_number::roblox_1011, 0, 32, 96, "Navy blue" );
            insert( brick_color::brick_number::roblox_1012, 33, 84, 185, "Deep blue" );
            insert( brick_color::brick_number::roblox_1013, 4, 175, 236, "Cyan" );
            insert( brick_color::brick_number::roblox_1014, 170, 85, 0, "CGA brown" );
            insert( brick_color::brick_number::roblox_1015, 170, 0, 170, "Magenta" );
            insert( brick_color::brick_number::roblox_1016, 255, 102, 204, "Pink" );
            insert( brick_color::brick_number::roblox_1017, 255, 175, 0, "Deep orange" );
            insert( brick_color::brick_number::roblox_1018, 18, 238, 212, "Teal" );
            insert( brick_color::brick_number::roblox_1019, 0, 255, 255, "Toothpaste" );
            insert( brick_color::brick_number::roblox_1020, 0, 255, 0, "Lime green" );
            insert( brick_color::brick_number::roblox_1021, 58, 125, 21, "Camo" );
            insert( brick_color::brick_number::roblox_1022, 127, 142, 100, "Grime" );
            insert( brick_color::brick_number::roblox_1023, 140, 91, 159, "Lavender" );
            insert( brick_color::brick_number::roblox_1024, 175, 221, 255, "Pastel light blue" );
            insert( brick_color::brick_number::roblox_1025, 255, 201, 201, "Pastel orange" );
            insert( brick_color::brick_number::roblox_1026, 177, 167, 255, "Pastel violet" );
            insert( brick_color::brick_number::roblox_1027, 159, 243, 233, "Pastel blue-green" );
            insert( brick_color::brick_number::roblox_1028, 204, 255, 204, "Pastel green" );
            insert( brick_color::brick_number::roblox_1029, 255, 255, 204, "Pastel yellow" );
            insert( brick_color::brick_number::roblox_1030, 255, 204, 153, "Pastel brown" );
            insert( brick_color::brick_number::roblox_1031, 98, 37, 209, "Royal purple" );
            insert( brick_color::brick_number::roblox_1032, 255, 0, 191, "Hot pink" );
        }
    };

    brick_color::brick_map& brick_color::brick_map::get()
    {
        static brick_map s;
        return s;
    }

    const std::string& brick_color::name()
    {
        const std::vector< brick_map::color_info >& colors = brick_map::get().map;
        return colors[ static_cast< std::int32_t >( number ) ].name;
    }

    ImColor brick_color::color()
    {
        const std::vector< brick_map::color_info >& colors = brick_map::get().map;

        assert(
            static_cast< unsigned int >( number ) < colors.size() && colors[ static_cast< std::int32_t >( number ) ].valid );

        return colors[ static_cast< std::int32_t >( number ) ].color;
    }
}  // namespace nord::rbx