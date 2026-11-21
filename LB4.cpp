#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <windows.h>
#include <locale>

using namespace std;

void clear_input() {
    cin.clear();
    cin.ignore(1000000, '\n');
}

double read_double(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;
        cout << "Невірне введення. Спробуйте ще раз.\n";
        clear_input();
    }
}

int read_int(const string& prompt, int minv = INT_MIN, int maxv = INT_MAX) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= minv && x <= maxv) return x;
        cout << "Невірне введення. Спробуйте ще раз.\n";
        clear_input();
    }
}

// ===== Перша задача =====
class GridTask {
private:
    int rows, cols;
    double* data;

public:
    GridTask() : rows(0), cols(0), data(nullptr) {}
    ~GridTask() { clear(); }

    void clear() {
        delete[] data;
        data = nullptr;
        rows = cols = 0;
    }

    void create(int r, int c) {
        clear();
        rows = r;
        cols = c;
        data = new double[rows * cols] {};
    }

    void setCell(int r, int c, double val) {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
            data[r * cols + c] = val;
    }

    double getCell(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
            return data[r * cols + c];
        return 0;
    }

    void inputField() {
        int r, c;
        cout << "Введіть кількість рядків і стовпців: ";
        while (!(cin >> r >> c) || r <= 0 || c <= 0) {
            cout << "Помилка. Спробуйте ще раз: ";
            clear_input();
        }

        create(r, c);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                setCell(i, j, read_double(
                    "cell[" + to_string(i) + "][" + to_string(j) + "] = "));
    }

    void printField() const {
        cout << fixed << setprecision(2);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                cout << setw(8) << getCell(i, j);
            cout << "\n";
        }
    }

    double solvePath(int r1, int c1, int r2, int c2) {
        if (r2 < r1 || c2 < c1) return -1;

        int R = r2 - r1 + 1;
        int C = c2 - c1 + 1;

        double* dp = new double[R * C];
        for (int i = 0; i < R * C; ++i) dp[i] = 1e300;

        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j) {
                double cost = getCell(r1 + i, c1 + j);
                if (i == 0 && j == 0) dp[0] = cost;
                else {
                    double best = 1e300;
                    if (i > 0) best = min(best, dp[(i - 1) * C + j]);
                    if (j > 0) best = min(best, dp[i * C + j - 1]);
                    if (i > 0 && j > 0) best = min(best, dp[(i - 1) * C + j - 1]);
                    dp[i * C + j] = cost + best;
                }
            }

        double ans = dp[R * C - 1];
        delete[] dp;
        return ans;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ukr");

    GridTask grid;
    bool run = true;

    while (run) {
        cout << "\n--- Меню ---\n";
        cout << "1) Ввести поле\n";
        cout << "2) Вивести поле\n";
        cout << "3) Змінити клітинку\n";
        cout << "4) Знайти мінімальний шлях\n";
        cout << "0) Вихід\n";

        int c = read_int("Ваш вибір: ", 0, 4);

        if (c == 0) break;
        if (c == 1) grid.inputField();
        else if (c == 2) grid.printField();
        else if (c == 3) {
            int r = read_int("x: ", 0);
            int col = read_int("y: ", 0);
            double v = read_double("Значення: ");
            grid.setCell(r, col, v);
        }
        else if (c == 4) {
            int r1, c1, r2, c2;
            cout << "r1 c1: "; cin >> r1 >> c1;
            cout << "r2 c2: "; cin >> r2 >> c2;
            double res = grid.solvePath(r1, c1, r2, c2);
            cout << "Мінімальна вартість: " << res << "\n";
        }
    }

    return 0;
}
