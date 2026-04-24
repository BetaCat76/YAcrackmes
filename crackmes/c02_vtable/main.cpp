#include <print>
#include <string>

// ── Base class ────────────────────────────────────────────────────────────────

class GameObj {
public:
    std::string name;
    int hp;
    int mp;

    GameObj(std::string name, int hp, int mp)
        : name(std::move(name)), hp(hp), mp(mp) {}

    virtual ~GameObj() = default;

    // Attack the target, dealing damage to its HP
    virtual void attack(GameObj& target) = 0;

    // Restore own MP
    virtual void restoreMp() = 0;

    bool isAlive() const { return hp > 0; }
};

// ── Player ────────────────────────────────────────────────────────────────────

class Player : public GameObj {
    static constexpr int kAttackDamage = 15;
    static constexpr int kMpCost       = 5;
    static constexpr int kMpRestore    = 20;

public:
    Player(std::string name, int hp, int mp)
        : GameObj(std::move(name), hp, mp) {}

    void attack(GameObj& target) override {
        if (mp < kMpCost) {
            std::println("[{}] Not enough MP to attack!", name);
            return;
        }
        mp -= kMpCost;
        target.hp -= kAttackDamage;
        std::println("[{}] attacks [{}] for {} damage. ({} HP left)",
                     name, target.name, kAttackDamage, target.hp);
    }

    void restoreMp() override {
        mp += kMpRestore;
        std::println("[{}] restores {} MP. ({} MP now)", name, kMpRestore, mp);
    }
};

// ── Monster ───────────────────────────────────────────────────────────────────

class Monster : public GameObj {
    static constexpr int kAttackDamage = 10;
    static constexpr int kHpRestore    = 5;   // monsters have no MP; they recover HP instead

public:
    // Monsters do not use MP; pass 0
    Monster(std::string name, int hp)
        : GameObj(std::move(name), hp, 0) {}

    void attack(GameObj& target) override {
        target.hp -= kAttackDamage;
        std::println("[{}] attacks [{}] for {} damage. ({} HP left)",
                     name, target.name, kAttackDamage, target.hp);
    }

    // Monsters have no MP; they restore a small amount of HP instead
    void restoreMp() override {
        hp += kHpRestore;
        std::println("[{}] regenerates {} HP. ({} HP now)", name, kHpRestore, hp);
    }
};

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    Player  player("Hero",   100, 50);
    Monster monster("Goblin", 60);

    std::println("=== Battle Start ===");
    std::println("Player  : {} | HP {} | MP {}", player.name,  player.hp,  player.mp);
    std::println("Monster : {} | HP {}", monster.name, monster.hp);
    std::println("");

    int round = 1;
    while (player.isAlive() && monster.isAlive()) {
        std::println("-- Round {} --", round++);

        player.attack(monster);
        if (!monster.isAlive()) break;

        monster.attack(player);
        if (!player.isAlive()) break;

        // Every 3 rounds each side recovers resources
        if (round % 3 == 0) {
            player.restoreMp();
            monster.restoreMp();
        }
        std::println("");
    }

    std::println("");
    std::println("=== Battle End ===");
    if (player.isAlive())
        std::println("{} wins!", player.name);
    else
        std::println("{} wins!", monster.name);

    return 0;
}
