#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using ld = long double;
using matrix = vector<vector<ld>>;

void print_matrix(const matrix &x) {
    for (const auto &line : x) {
        for (auto elem : line) {
            cout << elem << ' ';
        }
        cout << '\n';
    }
}

void print_vector(const vector<ld> &x) {
    for (auto i : x) {
        cout << i << ' ';
    }
    cout << '\n';
}

ld sum(matrix x, int i) {
    ld result = 0;
    for (auto &j : x) {
        result += pow(j[i], 2);
    }
    return result;
}

ld find_min(matrix x, int column) {
    ld min = numeric_limits<ld>::max();
    for (auto & i : x) {
        if (i[column] < min) {
            min = i[column];
        }
    }
    return min;
}

ld find_max(matrix x, int column) {
    ld max = numeric_limits<ld>::min();
    for (auto & i : x) {
        if (i[column] > max) {
            max = i[column];
        }
    }
    return max;
}

matrix calculate_normalized_matrix(const matrix &def_matrix) {
    matrix normalized_matrix(def_matrix.size(), vector<ld>(def_matrix[0].size()));
    for (int i = 0; i < def_matrix[0].size(); ++i) {
        for (int j = 0; j < def_matrix.size(); ++j) {
            normalized_matrix[j][i] = def_matrix[j][i] / sqrt(sum(def_matrix, i));
        }
    }
    return normalized_matrix;
}

matrix calculate_weights(const matrix &x, const vector<pair<int, ld>> &w) {
    matrix v(x.size(), vector<ld>(x[0].size()));
    for (int i = 0; i < x[0].size(); ++i) {
        for (int j = 0; j < x.size(); ++j) {
            v[j][i] = x[j][i] * w[i].second;
        }
    }
    return v;
}

vector<ld> calculate_ideal_best(const matrix& x, vector<pair<int, ld>> w) {
    vector<ld> ideals(w.size());
    for (int i = 0; i < w.size(); ++i) {
        if (w[i].first == 0) {
            ideals[i] = find_min(x, i);
        } else if (w[i].first == 1) {
            ideals[i] = find_max(x, i);
        }
    }
    return ideals;
}

vector<ld> calculate_ideal_worst(const matrix& x, vector<pair<int, ld>> w) {
    vector<ld> ideals(w.size());
    for (int i = 0; i < w.size(); ++i) {
        if (w[i].first == 1) {
            ideals[i] = find_min(x, i);
        } else if (w[i].first == 0) {
            ideals[i] = find_max(x, i);
        }
    }
    return ideals;
}

vector<ld> calculate_distance(matrix v, vector<ld> ideal) {
    vector<ld> distance(v.size());
    for (int i = 0; i < v.size(); ++i) {
        ld sum = 0;
        for (int j = 0; j < v[0].size(); ++j) {
            sum += pow((v[i][j] - ideal[j]), 2);
        }
        distance[i] = sqrt(sum);
    }
    return distance;
}

vector<ld> calculate_performance_score(vector<ld> distance_best, vector<ld> distance_worst) {
    vector<ld> p(distance_best.size());
    for (int i = 0; i < p.size(); ++i) {
        p[i] = distance_worst[i] / (distance_best[i] + distance_worst[i]);
    }
    return p;
}

int main() {
    matrix evaluation_matrix = {
            {250, 16, 12, 5},
            {200, 16, 8,  3},
            {300, 32, 16, 4},
            {275, 32, 8,  4},
            {225, 16, 16, 2},
    };
    vector<pair<int, ld>> weights = {{0, 0.35},
                                     {1, 0.25},
                                     {1, 0.25},
                                     {1, 0.15}};

    cout << "Evaluation matrix:\n";
    print_matrix(evaluation_matrix);


    cout << "Normalized:\n";
    evaluation_matrix = calculate_normalized_matrix(evaluation_matrix);
    print_matrix(evaluation_matrix);

    cout << "Weights:\n";
    evaluation_matrix = calculate_weights(evaluation_matrix, weights);
    print_matrix(evaluation_matrix);

    cout << "Ideal best:\n";
    vector<ld> best = calculate_ideal_best(evaluation_matrix, weights);
    print_vector(best);

    cout << "Ideal worst:\n";
    vector<ld> worst = calculate_ideal_worst(evaluation_matrix, weights);
    print_vector(worst);

    cout << "Distance for best:\n";
    vector<ld> distance_best = calculate_distance(evaluation_matrix, best);
    print_vector(distance_best);

    cout << "Distance for worst:\n";
    vector<ld> distance_worst = calculate_distance(evaluation_matrix, worst);
    print_vector(distance_worst);

    cout << "Scores:\n";
    vector<ld> scores = calculate_performance_score(distance_best, distance_worst);
    print_vector(scores);

    cout << "is the best choice!\n";
}