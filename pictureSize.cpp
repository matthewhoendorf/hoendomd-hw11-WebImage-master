/*
  pictureSize.cpp
  Matt Hoendorf
  2014/11/17
  
  This file demos a simple CGI program able to output a custom-made image, with
  a custom size.
  
  To use this code, create a cgi-bin directory, a images directory,
  and use the following command
  to compile in the current directory:

  $ g++ -o cgi-bin/pictureSize.cgi pictureSize.cpp  -lcgicc -lpthread
  
  Then start a simple HTTP Python server in the current directory:
  $ python -m CGIHTTPServer
  
  Then open a web browser and go to:
  http://localhost:8000/cgi-bin/pictureSize.cgi

*/

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"

#define cimg_display 0
#include "CImg.h"

using namespace std;


int main(int argc, char **argv){

  srand(time(0));
  
  int bands = rand() %100;

  // Generate the html page
  cgicc::Cgicc cgi;

  // Send HTTP header
  cout << cgicc::HTTPHTMLHeader() << endl;

  // Set up the HTML document
  cout << "<html>\n" << "<head><title>White Noise Image Generator</title></head>\n" << endl;
  cout << "<body><h1>White Noise Image Generator</h1>\n" << endl;

  cgicc::form_iterator width = cgi.getElement("mywidth");
  cgicc::form_iterator height = cgi.getElement("myheight");
  string input_w;
  string input_h;
    
  // Generate an image
  if(width != cgi.getElements().end() && height != cgi.getElements().end()) {
    
    input_w = **width;
    input_h = **height;

    cout << "<form method=\"GET\">\n";
    cout << "Width: <input type=\"text\" name=\"mywidth\" value=\"" << input_w << "\">\n";
    cout << "Height: <input type=\"text\" name=\"myheight\" value=\"" << input_h << "\">\n";
    cout << "<input type=\"submit\" value=\"Go!\">\n";
    cout << "</form>\n";
    
    int w = atoi((**width).c_str());
    int h = atoi((**height).c_str());

    cil::CImg<unsigned char> image(w,h,1, 3, 255); // Prefill image as white
    
    for(int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
          int num = rand() % 100 + 1;
          if (num < 50){
            image(x, y, 0) = 30;
            image(x, y, 1) = 30;
            image(x, y, 2) = 30;
          }
        }
      }

    // Save the image with a random string for this particular request
    int num = rand() % 1000;
    stringstream stream;
    stream << "images/image" << num << ".png";
    string filename = stream.str();
    
    // Saves the image relative to the running server, so images and cgi-bin should be subfolders.
    image.save(filename.c_str());
  
    // Include the image in the html, this is relative to the cgi-bin folder.
    cout << "<img src=\"../" << filename << "\" >" << endl;

  }

  else{
    
    input_w = "200";
    input_h = "200";

    cout << "<form method=\"GET\">\n";
    cout << "Width: <input type=\"text\" name=\"mywidth\" value=\"" << input_w << "\">\n";
    cout << "Height: <input type=\"text\" name=\"myheight\" value=\"" << input_h << "\">\n";
    cout << "<input type=\"submit\" value=\"Go!\">\n";
    cout << "</form>\n";
  }

  // Close the HTML document
  cout << "</body></html>\n";
  
  
  return 0;
}
