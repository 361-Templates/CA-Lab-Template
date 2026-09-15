#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "emp/math/Random.hpp"

// This hooks into the "target" element in index.html
// Must be outside of the CAAnimator class for javascript reasons
emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate {

// arena width and height
const int num_h_boxes = 10;
const int num_w_boxes = 15;
const double RECT_SIDE = 25;
const double width{num_w_boxes * RECT_SIDE}; // weird syntax for calculating an assigning a value
const double height{num_h_boxes * RECT_SIDE};

std::vector<std::vector<int> > cells; // How we make a 2D list
std::vector<std::vector<int> > cells_next;
        

// where we'll draw
emp::web::Canvas canvas{width, height, "canvas"};

public:

    CAAnimator() {
        // shove canvas into the div
        // along with a control button
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // make the lists the sizes we actually want
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));
        cells_next.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        // Setting up the starting glider
        cells[1][2] = 1;
        cells[2][3] = 1;
        cells[3][3] = 1;
        cells[3][1] = 1;
        cells[3][2] = 1;

        
    }

    // A gross method for counting living neighbors
    // But it demonstrates various C++ syntax things!
    int checkLiveNeighbors(int x, int y){
        int count_living = 0;

        for (int x_diff = -1; x_diff <=1; x_diff++) {
            for (int y_diff = -1; y_diff <= 1; y_diff++) {
                if (x_diff ==0 && y_diff == 0) continue; // Skip the focal cell

                int neighbor_x = x + x_diff;
                int neighbor_y = y + y_diff;

                // Handle wrapping, we could use % but we don't need it
                if (neighbor_x < 0 ) neighbor_x = num_w_boxes -1;
                else if (neighbor_x >= num_w_boxes) neighbor_x = 0;

                if (neighbor_y < 0) neighbor_y = num_h_boxes - 1;
                else if(neighbor_y >= num_h_boxes) neighbor_y = 0;

                // Count it!
                if (cells[neighbor_x][neighbor_y] == 1) {
                    count_living += 1;
                }
            }
        }

        return count_living;
        
    }

    void DoFrame() override {
        canvas.Clear();

        for (int x = 0; x < num_w_boxes; x++){
             for (int y = 0; y < num_h_boxes; y++) {
                int cell_value = cells[x][y];

                if (cells[x][y] == 0) {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                } else {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "black", "black");
                }
                
            }
        }



        for (int x = 0; x < num_w_boxes; x++){
            for (int y = 0; y < num_h_boxes; y++) {
                cells_next[x][y] = cells[x][y];

                int live_neighbors = checkLiveNeighbors(x, y);
                if (cells[x][y] == 0) {
                    if (live_neighbors == 3) {
                        cells_next[x][y] = 1;
                    }
                } else {
                    if (live_neighbors < 2) {
                        cells_next[x][y] = 0;
                    } else if (live_neighbors > 3) {
                        cells_next[x][y] = 0;
                    }
                }
            }
        }
        cells = cells_next;

    }


};

CAAnimator animator;

int main() {animator.Step();}