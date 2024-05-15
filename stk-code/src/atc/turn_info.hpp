enum TurnDirection {
    LEFT, RIGHT, STRAIGHT, NONE
};


// used when categorizing turns
enum TurnState {
    NO_TURN, IN_TURN, WAIT_END_TURN
};


struct TurnInfo {
    TurnInfo()
    {
        dir = NONE;
        intensity = 0;

        start_sector = -1;
        end_sector = -1;
    }

    TurnDirection dir;
    int intensity; // [0-6]

    int start_sector;
    int end_sector;
    
};

struct TurnBasics {
    static constexpr int TURN_INTENSITIES = 7;

    TurnBasics() {
        sector_start = -1;
        sector_end = -1;

        intensities = new int[TURN_INTENSITIES];
        for (int i = 0; i < TURN_INTENSITIES; ++i) {
            intensities[i] = 0;
        }
    }

    int sector_start;
    int sector_end;
    // number of sectors with each intensity of angle
    int *intensities;
};