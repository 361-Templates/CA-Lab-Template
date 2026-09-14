#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

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
        int cell_before = x-1;
        if(cell_before < 0) cell_before = 0;
        int cell_after = x + 1;
        if(cell_after>=num_w_boxes) cell_after = num_w_boxes-1;
        int cell_above =  y - 1;
        if (cell_above <0) cell_above = 0;
        int cell_below = y +1;
        if (cell_below >= num_h_boxes) {
            cell_below = num_h_boxes-1;
        }

        for (int i = cell_before; i <= cell_after; i++) {
            for (int j= cell_above; j <= cell_below; j++) {
                if(i!=x || j!=y){
                    if(cells[i][j]) {
                        count_living += 1;
                    }
                }
            }
        }

        //Might have missed some if wrapped
        if (cell_before == x) {
            int new_before = num_w_boxes-1;
            for (int j=cell_above; j <= cell_below; j++){
                if(cells[new_before][j]) count_living+=1;
            }
            if (cell_above == y && cells[new_before][num_h_boxes-1]) {
                count_living+=1;
            }
            else if (cell_below == y && cells[new_before][0]) count_living+=1;
        }
        if (cell_after == x) {
            int new_after = 0;
            for (int j=cell_above; j<=cell_below; j++) {
                if(cells[new_after][j]) count_living +=1;
            }
            if (cell_above == y && cells[new_after][num_h_boxes-1]) count_living+=1;
            else if (cell_below == y && cells[new_after][0]) count_living+=1;
            
        }

        if (cell_above == y) {
            int new_above = num_h_boxes -1;
            for (int i=cell_before; i <= cell_after; i++){
                if(cells[i][new_above]) count_living +=1;
            }
        }
        if (cell_below == y) {
            int new_below = 0;
            for (int i=cell_before; i <= cell_after; i++){
                if(cells[i][new_below]) count_living +=1;
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