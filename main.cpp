#include <bits/stdc++.h>
#define pb push_back
#define MAX 10000000001
using namespace std;

typedef vector<int> vi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
typedef long long int lli;

struct node{
    lli id;
    lli id_ship;
    lli distance;
    vi AdjNodes;
    string color = "";
};

struct typeShips{
    lli recognition = 0;
    lli frigates = 0;
    lli bombings = 0;
    lli conveyors = 0;
};

typedef vector<node> graph;
typedef vector<graph> group_graphs;
graph spaceship;
graph ship_find;
group_graphs ships_found;
vi ships_found_type; //1 = Reconhecimento; 2 = Frigata; 3 = Bombardeio; 4 = Transportadores
typeShips countShips;

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

lli BFS(graph g, int init, int end){

    for(int i = 0; i < g.size(); i++)
    {
        spaceship[g[i].id - 1].color.assign("WHITE");
        spaceship[g[i].id - 1].distance = 0;
    }

    node u;
    queue<node> q; q.push(spaceship[init]);

    while(!q.empty())
    {
        u = q.front(); q.pop();
        for(int i=0; i < u.AdjNodes.size(); i++)
        {
            if((spaceship[u.AdjNodes[i]].color == "WHITE") && (u.AdjNodes[i] != end))
            {
                spaceship[u.AdjNodes[i]].color = "GREY";
                spaceship[u.AdjNodes[i]].distance = u.distance + 1;
                q.push(spaceship[u.AdjNodes[i]]);
            }else if(u.AdjNodes[i] == end){
                spaceship[u.AdjNodes[i]].distance = u.distance + 1;
                return spaceship[u.AdjNodes[i]].distance;
            }
        }
        u.color = "BLACK";
    }
    return 0;
}

void find_distance(vvii teleport)
{
    lli distance, minor_distance = MAX;

    for(int i = 0; i < teleport.size(); i++) //Rodando o tempo de vantagem pra cada nave
    {
        distance = 0;
        for(int j = 0; j < teleport[i].size(); j++) //Calculando o tempo de vantagem para uma dada nave específica
        {
            if(distance <= minor_distance)
            {
                if(teleport[i][j].first - teleport[i][j].second != 0)
                {
                    if (ships_found_type[i] == 1) //Tempo de vantagem para as naves de reconhecimento
                        distance += abs(teleport[i][j].first - teleport[i][j].second);
                    else if (ships_found_type[i] == 4) //Tempo de vantagem para as naves transportadoras
                    {
                        if (abs(teleport[i][j].first - teleport[i][j].second) <
                            abs(teleport[i][j].second - teleport[i][j].first))
                            distance += abs(teleport[i][j].first - teleport[i][j].second);
                        else
                            distance += abs(teleport[i][j].second - teleport[i][j].first);
                    }else
                        distance += BFS(ships_found[i], teleport[i][j].first, teleport[i][j].second);
                }
            }
        }
        if(distance < minor_distance)  minor_distance = distance;
    }
    cout << minor_distance/2 << endl;
}

//###################################################################################################

void DFS_Visit(lli vertex, lli number_ship)
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
    lli var_number_ship = 0;
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

    while((i < g.size()) && (g[i].AdjNodes.size() <= 2) && (one_adj >= 0))
    {
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

void identification_ships(){
    int n_edge;
    for(int i = 0; i < ships_found.size(); i++)
    {
        n_edge = number_edge(ships_found[i]);
        if(ships_found[i].size() == n_edge)
        {
            countShips.conveyors++; //Transportadora -> N.Arestas = N.Vértices
            ships_found_type.pb(4);
        }
        else if(n_edge == (ships_found[i].size() - 1))
        {
            if(is_not_recognition(ships_found[i]))
            {
                countShips.frigates++;
                ships_found_type.pb(2);
            }
            else
            {
                countShips.recognition++;
                ships_found_type.pb(1);
            }
        }else
        {
            countShips.bombings++;
            ships_found_type.pb(3);
        }
    }
    cout << countShips.recognition << " " << countShips.frigates  << " " << countShips.bombings  << " " << countShips.conveyors << endl;
}

//###################################################################################################

int main()
{
    //auto start = chrono::high_resolution_clock::now(); //Início do cálculo do tempo de processamento do programa
    int N, M;
    int from, arrival;

    //Leitura dos tamanhos da entrada
    cin >> N >> M;
    spaceship.resize(N);

    //Armazenando o grafo de entrada
    for(int i = 0; i < M; i++)
    {
        cin >> from >> arrival;
        spaceship[from-1].AdjNodes.pb(arrival-1);
        spaceship[arrival-1].AdjNodes.pb(from-1);
    }
    DFS_NumberOfSpaceships(N); //Aplicando o DFS para identificar o número de naves da tropa inimiga
    identification_ships(); //Detectando cada tipo de nave inimiga

    vvii teleport(ships_found.size());
    ii aux;

    //Lendo os teleportes
    for(int i = 0; i < N; i++)
    {
        cin >> from >> arrival;
        aux.first = from - 1;
        aux.second = arrival - 1;
        teleport[spaceship[from - 1].id_ship].pb(aux);
    }
    find_distance(teleport); //Calculando o tempo de vantagem

    //auto finish = chrono::high_resolution_clock::now();
    //chrono::duration<double> elapsed = finish - start;
    //cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}