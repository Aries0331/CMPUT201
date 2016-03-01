/*  ONE Card number: 1461696
 Computing Science UNIX ID: jinzhu
 lecture section: A2
 lab section: D01
 TA's name: Chenyang Huang  */

#define MAX_POINTS 20
#define T 0.05

void drawLandscape(FILE * input, FILE * lander);
void draw_ship(FILE * lander);
void erase_ship(FILE * lander);
void draw_thrust(FILE * lander);
void erase_thrust(FILE * lander);
void rotate_left();
void rotate_right();
void active_thrust(FILE * lander);
void ship_fall();
void handle_timeout(int signal);
void comm_line_argu(int argc, char *argv[]);
void draw_crash();
void draw_landed();
void draw_fuel();
void erase_fuel();
void explode();
int out_of_window();

FILE * input;
FILE * lander;

int num, fuel = 0, reduce = 0;
int running, c, Gravity=0, Thrust=0, Improve=0;
double center_x, center_y;
double angle_degrees, angle_radians;
double rotate_radius_R, rotate_radius_L;
double xA, yA, gravity, thrust;
double Ax,Ay,Bx,By,Cx,Cy,Dx,Dy;
double new_velocity_x, old_velocity_x, new_velocity_y, old_velocity_y;
char *line;

struct sigaction handler;

struct coordinates{
    double x;
    double y;
};

struct coordinates points[MAX_POINTS];
struct coordinates ship_coordinates[6];
struct coordinates thrust_coordinates[3];

sigset_t block_mask;
sigset_t old_mask;



