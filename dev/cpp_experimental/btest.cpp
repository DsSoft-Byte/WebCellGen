#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

struct Vec2 {
    double x, y;
};

int main(int argc, char** argv) {

    /* ===== USER PARAMETERS ===== */
    int series_cells   = 4;
    int parallel_cells = 6;
    bool honeycomb     = true;

    double cell_d      = 18.3;
    double adjust      = 0.2;
    double wall        = 5.0;
    double height      = 12.0;
    double corner_r    = 8.0;

    double wire_d      = 3.0;     // balancer wire hole
    double ring_h      = 0.4;     // insulation ring height

	double hole_offset_top = 0.312;
    /* =========================== */

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
		// if (arg == "help" && i + 1 < argc) {
		// 	std::cout << "--series_cells (default: 4) \n" 
		// 				 "--parallel_cells (default: 6)\n"
		// 				 "--honeycomb (default: true) \n"
		// 				 "--no-honeycomb (default: false)";
		// }

		if (arg == "--series_cells" && i + 1 < argc) {
			series_cells = std::stoi(argv[++i]);
		}
    
        else if (arg == "--parallel_cells" && i + 1 < argc) {
            parallel_cells = std::stoi(argv[++i]);
        }
        else if (arg == "--honeycomb") {
            honeycomb = true;
        }
        else if (arg == "--no-honeycomb") {
            honeycomb = false;
        }
    }
    

    double d = cell_d + adjust;
    double r = d * 0.5;
    double spacing = d * 1.042;

    double row_h = honeycomb ? spacing * 0.866 : spacing;

    double body_w =
        (series_cells - 1) * spacing +
        d +
        (honeycomb ? spacing * 0.5 : 0.0) +
        2 * wall;

    double body_h =
        (parallel_cells - 1) * row_h +
        d +
        2 * wall;

    double hole_r = wire_d * 0.5;

    /* ===== CELL CENTER GENERATION ===== */
	
    auto cell_center = [&](int x, int y) {
        double ox = (honeycomb && (y & 1)) ? spacing * 0.5 : 0.0;
        return Vec2{
            x * spacing + ox + r + wall,
            y * row_h + r + wall
        };
    };


/* ===== GAP X POSITIONS ===== */
std::vector<Vec2> balancer_holes;

if(!honeycomb) {
    auto gap_x_for_row = [&](int row) {
        std::vector<double> xs;
        int max_gaps = honeycomb ? series_cells : series_cells + 1;
        for (int i = 0; i < max_gaps; i++) {
            double ox = (honeycomb && (row & 1)) ? spacing * 0.5 : 0.0;
            double x = i * spacing + r + wall + spacing * 0.5 + ox;
            // Only filter if actually outside the body bounds
            if (x >= hole_r && x <= body_w - hole_r)
                xs.push_back(x);
        }
        return xs;
};
    /* ===== BALANCER HOLES ===== */

    int top_row    = 0;
    int bottom_row = parallel_cells - 1;

    double y_top    = body_h - wall - hole_r;
    double y_bottom = wall + hole_r;

    for (double x : gap_x_for_row(top_row))
        balancer_holes.push_back({x, y_top});

    for (double x : gap_x_for_row(bottom_row))
        balancer_holes.push_back({x, y_bottom});

}
else{
	auto gap_x_positions = [&](double offset) {
	    std::vector<double> xs;
	
	    int gap_count = series_cells + 1;  // holes at edges and between cells
	
	    double start_x = (wall + r) * offset;   // start at the left edge + radius
	    double end_x = body_w - wall - r + offset;  // right edge - radius
	
	    for (int i = 0; i < gap_count; i++) {
	        // evenly space holes between start_x and end_x
	        double x = start_x + i * (end_x - start_x) / (gap_count - 1);
	        xs.push_back(x);
	    }
	    return xs;
	};
	
	
	    /* ===== BALANCER HOLES ===== */
		bool is_parallel_even = (parallel_cells % 2 == 0);
		
		auto gaps_bottom = gap_x_positions(0.312);
		auto gaps_top = gap_x_positions(is_parallel_even ? hole_offset_top : 0.312);

	
	    double y_top    = body_h - wall - hole_r + 2.5 ;
	    double y_bottom = wall + hole_r - 2.5;
	
	    for (double x : gaps_top)
	        balancer_holes.push_back({x + (is_parallel_even ? + 9.5 : 0), y_top});
	
	    for (double x : gaps_bottom)
	        balancer_holes.push_back({x, y_bottom});
}       
    /* ===== WRITE OPENSCAD ===== */
    std::ofstream scad("battery_spacer.scad");

    scad << "module cell(){cylinder(h=" << height+2
         << ",d=" << d << ",$fn=100);} \n";

    scad << "module ring(){difference(){"
         << "cylinder(h=" << ring_h << ",d=" << d+0.5 << ",$fn=100);"
         << "cylinder(h=" << ring_h << ",d=" << d*0.6 << ",$fn=100);"
         << "}} \n";

    scad << "module bh(){cylinder(h=" << height+2
         << ",d=" << wire_d << ",$fn=40);} \n";

    scad << "difference(){\n";
    scad << "linear_extrude(" << height << ") "
         << "offset(r=" << corner_r << ")"
         << "offset(delta=-" << corner_r << ")"
         << "square([" << body_w << "," << body_h << "]);\n";

    /* cell holes */
    for (int y = 0; y < parallel_cells; y++)
        for (int x = 0; x < series_cells; x++) {
            auto c = cell_center(x, y);
            scad << "translate([" << c.x << "," << c.y
                 << ",-1])cell();\n";
        }

    /* balancer holes */
    for (auto& h : balancer_holes)
        scad << "translate([" << h.x << "," << h.y
             << ",-1])bh();\n";

    scad << "}\n";

    /* upper insulation rings */
    for (int y = 0; y < parallel_cells; y++)
        for (int x = 0; x < series_cells; x++) {
            auto c = cell_center(x, y);
            scad << "translate([" << c.x << "," << c.y
                 << "," << height - ring_h
                 << "])ring();\n";
        }

    scad.close();

    std::cout << "Series cells:   " << series_cells << "\n";
        std::cout << "Parallel cells: " << parallel_cells << "\n";
        std::cout << "Honeycomb:        " << (honeycomb ? "yes" : "no") << "\n";
}

