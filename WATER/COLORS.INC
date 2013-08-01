// Persistence of Vision Raytracer
// Colors.inc
//
// Many pre-defined colors for use in scene files.
// Also includes constants for Map Types and Interpolation Types
// for use with image_map, bump_map, and material_map

//
// These grays are useful for fine-tuning lighting color values
// and for other areas where subtle variations of grays are needed.
#declare Gray05 = color red 0.05 green 0.05 blue 0.05
#declare Gray10 = color red 0.10 green 0.10 blue 0.10
#declare Gray15 = color red 0.15 green 0.15 blue 0.15
#declare Gray20 = color red 0.20 green 0.20 blue 0.20
#declare Gray25 = color red 0.25 green 0.25 blue 0.25
#declare Gray30 = color red 0.30 green 0.30 blue 0.30
#declare Gray35 = color red 0.35 green 0.35 blue 0.35
#declare Gray40 = color red 0.40 green 0.40 blue 0.40
#declare Gray45 = color red 0.45 green 0.45 blue 0.45
#declare Gray50 = color red 0.50 green 0.50 blue 0.50
#declare Gray55 = color red 0.55 green 0.55 blue 0.55
#declare Gray60 = color red 0.60 green 0.60 blue 0.60
#declare Gray65 = color red 0.65 green 0.65 blue 0.65
#declare Gray70 = color red 0.70 green 0.70 blue 0.70
#declare Gray75 = color red 0.75 green 0.75 blue 0.75
#declare Gray80 = color red 0.80 green 0.80 blue 0.80
#declare Gray85 = color red 0.85 green 0.85 blue 0.85
#declare Gray90 = color red 0.90 green 0.90 blue 0.90
#declare Gray95 = color red 0.95 green 0.95 blue 0.95

#declare DimGray = colour red 0.329412 green 0.329412 blue 0.329412
#declare DimGrey = colour red 0.329412 green 0.329412 blue 0.329412
#declare Gray = colour red 0.752941 green 0.752941 blue 0.752941
#declare Grey = colour red 0.752941 green 0.752941 blue 0.752941
#declare LightGray = colour red 0.658824 green 0.658824 blue 0.658824
#declare LightGrey = colour red 0.658824 green 0.658824 blue 0.658824
#declare VLightGrey = color red 0.80 green 0.80 blue 0.80

#declare Clear = colour red 1.0 green 1.0 blue 1.0 alpha 1.0
#declare White = colour red 1.0 green 1.0 blue 1.0
#declare Red = colour red 1.0
#declare Green = colour green 1.0
#declare Blue = colour blue 1.0
#declare Yellow = colour red 1.0 green 1.0
#declare Cyan = colour blue 1.0 green 1.0
#declare Magenta = colour red 1.0 blue 1.0
#declare Black = colour red 0.0 green 0.0 blue 0.0
#declare Aquamarine = colour red 0.439216 green 0.858824 blue 0.576471
#declare BlueViolet = colour red 0.62352 green 0.372549 blue 0.623529
#declare Brown = colour red 0.647059 green 0.164706 blue 0.164706
#declare CadetBlue = colour red 0.372549 green 0.623529 blue 0.623529
#declare Coral = colour red 1.0 green 0.498039 blue 0.0
#declare CornflowerBlue = colour red 0.258824 green 0.258824 blue 0.435294
#declare DarkGreen = colour red 0.184314 green 0.309804 blue 0.184314
#declare DarkOliveGreen = colour red 0.309804 green 0.309804 blue 0.184314
#declare DarkOrchid = colour red 0.6 green 0.196078 blue 0.8
#declare DarkSlateBlue = colour red 0.419608 green 0.137255 blue 0.556863
#declare DarkSlateGray = colour red 0.184314 green 0.309804 blue 0.309804
#declare DarkSlateGrey = colour red 0.184314 green 0.309804 blue 0.309804
#declare DarkTurquoise = colour red 0.439216 green 0.576471 blue 0.858824
#declare Firebrick = colour red 0.556863 green 0.137255 blue 0.137255
#declare ForestGreen = colour red 0.137255 green 0.556863 blue 0.137255
#declare Gold = colour red 0.8 green 0.498039 blue 0.196078
#declare Goldenrod = colour red 0.858824 green 0.858824 blue 0.439216
#declare GreenYellow = colour red 0.576471 green 0.858824 blue 0.439216
#declare IndianRed = colour red 0.309804 green 0.184314 blue 0.184314
#declare Khaki = colour red 0.623529 green 0.623529 blue 0.372549
#declare LightBlue = colour red 0.74902 green 0.847059 blue 0.847059
#declare LightSteelBlue = colour red 0.560784 green 0.560784 blue 0.737255
#declare LimeGreen = colour red 0.196078 green 0.8 blue 0.196078
#declare Maroon = colour red 0.556863 green 0.137255 blue 0.419608
#declare MediumAquamarine = colour red 0.196078 green 0.8 blue 0.6
#declare MediumBlue = colour red 0.196078 green 0.196078 blue 0.8
#declare MediumForestGreen = colour red 0.419608 green 0.556863 blue 0.137255
#declare MediumGoldenrod = colour red 0.917647 green 0.917647 blue 0.678431
#declare MediumOrchid = colour red 0.576471 green 0.439216 blue 0.858824
#declare MediumSeaGreen = colour red 0.258824 green 0.435294 blue 0.258824
#declare MediumSlateBlue = colour red 0.498039 blue 1.0
#declare MediumSpringGreen = colour red 0.498039 green 1.0 
#declare MediumTurquoise = colour red 0.439216 green 0.858824 blue 0.858824
#declare MediumVioletRed = colour red 0.858824 green 0.439216 blue 0.576471
#declare MidnightBlue = colour red 0.184314 green 0.184314 blue 0.309804
#declare Navy = colour red 0.137255 green 0.137255 blue 0.556863
#declare NavyBlue = colour red 0.137255 green 0.137255 blue 0.556863
#declare Orange = colour red 1 green 0.5 blue 0.0
#declare OrangeRed = colour red 1.0 blue 0.498039
#declare Orchid = colour red 0.858824 green 0.439216 blue 0.858824
#declare PaleGreen = colour red 0.560784 green 0.737255 blue 0.560784
#declare Pink = colour red 0.737255 green 0.560784 blue 0.560784
#declare Plum = colour red 0.917647 green 0.678431 blue 0.917647
#declare Salmon = colour red 0.435294 green 0.258824 blue 0.258824
#declare SeaGreen = colour red 0.137255 green 0.556863 blue 0.419608
#declare Sienna = colour red 0.556863 green 0.419608 blue 0.137255
#declare SkyBlue = colour red 0.196078 green 0.6 blue 0.8
#declare SlateBlue = colour green 0.498039 blue 1.0
#declare SpringGreen = colour green 1.0 blue 0.498039
#declare SteelBlue = colour red 0.137255 green 0.419608 blue 0.556863
#declare Tan = colour red 0.858824 green 0.576471 blue 0.439216
#declare Thistle = colour red 0.847059 green 0.74902 blue 0.847059
#declare Turquoise = colour red 0.678431 green 0.917647 blue 0.917647
#declare Violet = colour red 0.309804 green 0.184314 blue 0.309804
#declare VioletRed = colour red 0.8 green 0.196078 blue 0.6
#declare Wheat = colour red 0.847059 green 0.847059 blue 0.74902
#declare YellowGreen = colour red 0.6 green 0.8 blue 0.196078
#declare SummerSky = color red 0.22 green 0.69 blue 0.87
#declare RichBlue = color red 0.35 green 0.35 blue 0.67
#declare Brass =  colour red 0.71 green 0.65 blue 0.26
#declare Copper = colour red 0.72 green 0.45 blue 0.20
#declare Bronze = colour red 0.55 green 0.47 blue 0.14
#declare Bronze2 = colour red 0.65 green 0.49 blue 0.24
#declare Silver = colour red 0.90 green 0.91 blue 0.98
#declare BrightGold = color red 0.85 green 0.85 blue 0.10
#declare OldGold =  colour red 0.81 green 0.71 blue 0.23
#declare Feldspar = colour red 0.82 green 0.57 blue 0.46
#declare Quartz = color red 0.85 green 0.85 blue 0.95
#declare Mica = color Black  // needed in textures.inc
#declare NeonPink = color red 1.00 green 0.43 blue 0.78
#declare DarkPurple = color red 0.53 green 0.12 blue 0.47
#declare NeonBlue = color red 0.30 green 0.30 blue 1.00
#declare CoolCopper = color red 0.85 green 0.53 blue 0.10
#declare MandarinOrange = color red 0.89 green 0.47 blue 0.20
#declare LightWood = color red 0.91 green 0.76 blue 0.65
#declare MediumWood = color red 0.65 green 0.50 blue 0.39
#declare DarkWood = color red 0.52 green 0.37 blue 0.26
#declare SpicyPink = color red 1.00 green 0.11 blue 0.68
#declare SemiSweetChoc = color red 0.42 green 0.26 blue 0.15
#declare BakersChoc = color red 0.36 green 0.20 blue 0.09
#declare Flesh = color red 0.96 green 0.80 blue 0.69
#declare NewTan = color red 0.92 green 0.78 blue 0.62
#declare NewMidnightBlue = color red 0.00 green 0.00 blue 0.61
#declare VeryDarkBrown = color red 0.35 green 0.16 blue 0.14
#declare DarkBrown = color red 0.36 green 0.25 blue 0.20
#declare DarkTan = color red 0.59 green 0.41 blue 0.31
#declare GreenCopper = color red 0.32 green 0.49 blue 0.46
#declare DkGreenCopper = color red 0.29 green 0.46 blue 0.43
#declare DustyRose = color red 0.52 green 0.39 blue 0.39
#declare HuntersGreen = color red 0.13 green 0.37 blue 0.31
#declare Scarlet = color red 0.55 green 0.09 blue 0.09

// Map types constants
// for use with image_map, bump_map, and material_map
//
// Format:
//   map_type Sphere_Map or map_type Torus_Map
//
#declare Plane_Map = 0
#declare Sphere_Map = 1
#declare Cylinder_Map = 2
#declare Torus_Map = 5

// Interpolation constants
// Use in image_map and bump_map in form:
//
// interpolate Bi or interpolate Norm
//
#declare Bi   = 2    // Bilinear interpolation is best
#declare Norm = 4    // Normalized distance is a bit faster

