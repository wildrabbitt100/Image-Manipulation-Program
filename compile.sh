
cd C:/allegro_picture_blender


g++ -g main.cpp shade_coloured_picture.cpp merge_black_and_white.cpp text_output_management.cpp translucent_shading.cpp load_bitmaps.cpp make_negative_of_black_and_white_picture.cpp -o  colourer.exe -lallegro-5.0.5-monolith-md 

read -p "compiled code into colourer.exe . press a key.."
