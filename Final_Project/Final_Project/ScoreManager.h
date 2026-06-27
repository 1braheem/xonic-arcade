#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

class ScoreManager {
private:
    int score;
    int rewardCounter;
    int powerUps;
    int nextPowerUpScore;

public:
    ScoreManager() {
        score = 0;
        rewardCounter = 0;
        powerUps = 0;
        nextPowerUpScore = 50; //initiallizing nextPowerUpScore with 50 to get first powerup when your scre is 50
    }

    // Call this each time a region is captured
    void addCapturedTiles(int tilesCaptured) {
        int points = 0;

        // Determine multiplier(means it check conditions that the score should multiply with 4 or 2)
        int threshold;
        if (rewardCounter >= 3) {
            threshold = 5;
        }
        else {
            threshold = 10;
        }

        if (tilesCaptured > threshold) {
            if (rewardCounter >= 5)
                points = tilesCaptured * 4;
            else
                points = tilesCaptured * 2;

            rewardCounter++;
        }
        else {
            points = tilesCaptured;  // Normal 1 point per tile
        }

        score += points;

        checkForPowerUp();
    }

    // If a powerup is used decrease the poweup count by 1
    bool usePowerUp() {
        if (powerUps > 0) {
            powerUps--;
            return true;
        }
        return false;
    }

    // To update the score
    void updateScore(int captured) {
        addCapturedTiles(captured);
    }

    // Setter
    void setScore(int s) {
        score = s;
    }

    void setPowerUps(int p) {
        powerUps = p;
    }

    // Getters
    int getScore() const {
        return score;
    }

    int getRewardCounter() const {
        return rewardCounter;
    }

    int getPowerUps() const {
        return powerUps;
    }
private:
    // If condition to increase the powerup count meets, it increases the powerup count.
    void checkForPowerUp() {
        while (score >= nextPowerUpScore) {
            powerUps++;
            nextPowerUpScore += 30;
        }
    }
};

#endif