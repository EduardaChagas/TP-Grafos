#include <bits/stdc++.h>
#define pb push_back
#define MAX 1000001
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef long long int lli;

struct node{
    int id;
    int id_ship;
    int distance;
    vi AdjNodes;
    string color = "";
};

typedef vector<node> graph;
typedef vector<graph> group_graphs;
graph spaceship;
graph ship_find;
group_graphs ships_found;
vvi distances_stations;

void print_graph(graph g){
    printf("Começo \n");
    for(int i=0; i<g.size();i++){
        cout << "ID: " << g[i].id << endl;
    }
    printf("\n");
}

void print_group_graph(group_graphs g){
    for(int i = 0; i < g.size(); i++){
        cout << "Ship: " << i + 1 << endl;
        for(int j = 0; j < g[i].size(); j++){
            cout << " Station: " << g[i][j].id << endl;
        }
        printf("\n");
    }
}

//###################################################################################################

int BFS(graph g, int init, int end){
    for(int i = 0; i < g.size(); i++)
    {
        g[i].color.assign("WHITE");
        //g[i].distance = 0;
    }

    node u;
    queue<node> q; q.push(g[init]);

    while(!q.empty())
    {
        u = q.front(); q.pop();

        for(int i=0; i < u.AdjNodes.size(); i++)
        {
            if((g[u.AdjNodes[i]].color == "WHITE") && (u.AdjNodes[i] != end))
            {
                g[u.AdjNodes[i]].color = "GREY";
                if(distances_stations[init][u.AdjNodes[i]] == MAX) {
                    distances_stations[init][u.AdjNodes[i]] = distances_stations[init][u.id - 1] + 1;
                    distances_stations[u.AdjNodes[i]][init] = distances_stations[u.id - 1][init] + 1;
                }
                //cout << "OI " << u.AdjNodes[i] << " Distance: " << distances_stations[init][u.AdjNodes[i]] << endl;
                //g[u.AdjNodes[i]].distance = u.distance + 1;
                q.push(g[u.AdjNodes[i]]);
            }else if(u.AdjNodes[i] == end){
                distances_stations[init][end] = distances_stations[init][u.id - 1] + 1;
                distances_stations[end][init] = distances_stations[u.id - 1][init] + 1;
                return 0;
                //cout << "OI CHEGUEI " << g[u.AdjNodes[i]].id << " - Distance of my ancestor: " << u.distance << endl;
                //g[u.AdjNodes[i]].distance = u.distance + 1;
                //return g[u.AdjNodes[i]].distance;
            }
        }
        u.color = "BLACK";

    }
    return 0;
}

int minor_distance(vi distances, int N)
{
    int minor = distances[0];
    for(int i = 1; i < N;i++)
    {
        if(distances[i] < minor) minor = distances[i];
    }
    return minor;
}

void find_distance(int N, vii teleport)
{
    int ship_index, minor, distance;
    vi distances_ship(ships_found.size());
    vvi d(spaceship.size(),vi(spaceship.size()));
    distances_stations = d;

    for(int i = 0; i < spaceship.size(); i++)
    {
        for(int j = 0; j < spaceship.size(); j++)
        {
            if(i != j) distances_stations[i][j] = MAX;
            else distances_stations[i][j] = 0;
        }
    }
    for(int i = 0; i < ships_found.size(); i++)
    {
        distances_ship[i] = 0;
    }

    for(int i = 0; i < N; i++)
    {
        ship_index = spaceship[teleport[i].first].id_ship;
        if(distances_stations[teleport[i].first][teleport[i].second] == MAX)
        {
            BFS(spaceship, teleport[i].first, teleport[i].second);
            distances_ship[ship_index] += distances_stations[teleport[i].first][teleport[i].second];
        }else distances_ship[ship_index] += distances_stations[teleport[i].first][teleport[i].second];
        //cout <<"INIT: " << teleport[i].first + 1 << " END: " << teleport[i].second + 1 << " DISTANCE: " <<  distances_ship[ship_index] << endl;
        //printf("\n");
    }
    minor = minor_distance(distances_ship, ships_found.size());
    cout << minor/2 << endl;
}

//###################################################################################################

void DFS_Visit(int vertex, int number_ship)
{
    spaceship[vertex].color = "GREY";
    for(int i = 0; i < spaceship[vertex].AdjNodes.size(); i++)
    {
        if(spaceship[spaceship[vertex].AdjNodes[i]].color.compare("WHITE") == 0)
        {
            spaceship[spaceship[vertex].AdjNodes[i]].id_ship = number_ship;
            DFS_Visit(spaceship[vertex].AdjNodes[i], number_ship);
            ship_find.pb(spaceship[spaceship[vertex].AdjNodes[i]]);
        }
    }
    spaceship[vertex].color = "BLACK";
}

//Modificação do DFS para encontrar componentes conexas utilizando variável adicional
int DFS_NumberOfSpaceships(int n_vertex)
{
    int var_number_ship = 0;
    for(int i = 0; i < n_vertex; i++) //Inicializando os vértices
    {
        spaceship[i].color.assign("WHITE");
        spaceship[i].id = i + 1;
    }
    for(int i = 0; i < n_vertex; i++)
    {
        if(spaceship[i].color.compare("WHITE") == 0) //Em cada execução é encontrada uma componente conexa
        {
            spaceship[i].id_ship = var_number_ship;
            ship_find.resize(0); //Inicializando um novo grafo para alocar a nova nave detectada
            ship_find.pb(spaceship[i]);
            DFS_Visit(i, var_number_ship); //Realizando DFS apartir do vértice i
            ships_found.pb(ship_find); //Adicionando a nave encontrada no conjunto de naves do problema
            var_number_ship++;
        }
    }
    return 0;
}

//###################################################################################################

bool is_not_recognition(graph g){
    int i = 0;
    int one_adj = 2;

    while((i < g.size()) && (g[i].AdjNodes.size() <= 2) && (one_adj >= 0)){
        i++;
        if(g[i].AdjNodes.size() == 1) one_adj--;
    }

    if(i < g.size()) return true;

    return false;
}

int number_edge(graph g){
    int n = 0;

    for(int i=0; i < g.size(); i++)
        n = n + g[i].AdjNodes.size();

    n = n/2;

    return n;
}

void identification_ships(group_graphs gg){
    int recognition = 0, frigates = 0, bombings = 0, conveyors = 0, n_edge;
    for(int i = 0; i < gg.size(); i++){
        n_edge = number_edge(gg[i]);
        if(gg[i].size() == n_edge) conveyors++; //Transportadora -> N.Arestas = N.Vértices
        else if(n_edge == (gg[i].size() - 1)){
            if(is_not_recognition(gg[i])) frigates++;
            else recognition++;
        }else bombings++;
    }
    cout << recognition << " " << frigates  << " " << bombings  << " " << conveyors << endl;
}

//###################################################################################################

int main()
{
    auto start = chrono::high_resolution_clock::now(); //Início do cálculo do tempo de processamento do programa
    int N, M;
    int from, arrival;
    vii teleport;

    //Leitura dos tamanhos da entrada
    cin >> N >> M;
    spaceship.resize(N);
    teleport.resize(N);

    //Armazenando o grafo de entrada
    for(int i = 0; i < M; i++)
    {
        cin >> from >> arrival;
        spaceship[from-1].AdjNodes.pb(arrival-1);
        spaceship[arrival-1].AdjNodes.pb(from-1);
    }
    //Lendo os teleportes
    for(int i = 0; i < N; i++)
    {
        cin >> from >> arrival;
        teleport[i].first = from - 1;
        teleport[i].second = arrival - 1;
    }

    DFS_NumberOfSpaceships(N); //Aplicando o DFS para identificar o número de naves da tropa inimiga
    identification_ships(ships_found); //Detectando cada tipo de nave inimiga
    find_distance(N, teleport); //Calculando o tempo de vantagem

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}