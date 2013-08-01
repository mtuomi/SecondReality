// Persistence of Vision Raytracer
//
// Direction Vectors for various Field of View angles.
// Use in camera in form :
//
//     direction <0 0 FoV_45>
//
// The formula used to calculate these is:  FoV = 0.5 / tan(angle/2)
// You will also need to adjust the location vector if you change FoV and
// want to keep the same visual distance from your scene.
 
 #declare FoV_15 = 7.595981
 #declare FoV_30 = 3.732166
 #declare FoV_45 = 2.414293
 #declare FoV_60 = 1.732113
 #declare FoV_75 = 1.303277
 #declare FoV_90 = 1.000046
 #declare FoV_105 = 0.767370
 #declare FoV_120 = 0.577391
 #declare FoV_135 = 0.414254
 #declare FoV_150 = 0.267991
 #declare FoV_165 = 0.131696

