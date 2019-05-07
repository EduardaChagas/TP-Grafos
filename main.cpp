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
    lli idShip;
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
typedef vector<graph> GroupGraphs;
graph Spaceship;
graph ShipFind;
GroupGraphs ShipsFound;
vi shipsFoundType; //1 = Reconhecimento; 2 = Frigata; 3 = Bombardeio; 4 = Transportadores
typeShips countShips;

void print_graph(graph g){
    printf("Começo \n");
    for(int i=0; i<g.size();i++){
        cout << "ID: " << g[i].id << endl;
    }
    printf("\n");
}

void print_group_graph(GroupGraphs g){
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
        Spaceship[g[i].id - 1].color.assign("WHITE");
        Spaceship[g[i].id - 1].distance = 0;
    }

    node u;
    queue<node> q; q.push(Spaceship[init]);

    while(!q.empty())
    {
        u = q.front(); q.pop();
        for(int i=0; i < u.AdjNodes.size(); i++)
        {
            if((Spaceship[u.AdjNodes[i]].color == "WHITE") && (u.AdjNodes[i] != end))
            {
                Spaceship[u.AdjNodes[i]].color = "GREY";
                Spaceship[u.AdjNodes[i]].distance = u.distance + 1;
                q.push(Spaceship[u.AdjNodes[i]]);
            }else if(u.AdjNodes[i] == end){
                Spaceship[u.AdjNodes[i]].distance = u.distance + 1;
                return Spaceship[u.AdjNodes[i]].distance;
            }
        }
        u.color = "BLACK";
    }
    return 0;
}

void findDistance(vvii teleport)
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
                    if (shipsFoundType[i] == 1) //Tempo de vantagem para as naves de reconhecimento
                        distance += abs(teleport[i][j].first - teleport[i][j].second);
                    else if (shipsFoundType[i] == 4) //Tempo de vantagem para as naves transportadoras
                    {
                        if (abs(teleport[i][j].first - teleport[i][j].second) <
                            abs(teleport[i][j].second - teleport[i][j].first))
                            distance += abs(teleport[i][j].first - teleport[i][j].second);
                        else
                            distance += abs(teleport[i][j].second - teleport[i][j].first);
                    }else
                        distance += BFS(ShipsFound[i], teleport[i][j].first, teleport[i][j].second);
                }
            }
        }
        if(distance < minor_distance)  minor_distance = distance;
    }
    cout << minor_distance/2 << endl;
}

//###################################################################################################

void DFSVisit(lli vertex, lli numberShip)
{
    Spaceship[vertex].color = "GREY";
    for(int i = 0; i < Spaceship[vertex].AdjNodes.size(); i++)
    {
        if(Spaceship[Spaceship[vertex].AdjNodes[i]].color.compare("WHITE") == 0)
        {
            Spaceship[Spaceship[vertex].AdjNodes[i]].idShip = numberShip;
            DFSVisit(Spaceship[vertex].AdjNodes[i], numberShip);
            ShipFind.pb(Spaceship[Spaceship[vertex].AdjNodes[i]]);
        }
    }
    Spaceship[vertex].color = "BLACK";
}

//Modificação do DFS para encontrar componentes conexas utilizando variável adicional
int DFSNumberOfSpaceships(int nVertex)
{
    lli varNumberShip = 0;
    for(int i = 0; i < nVertex; i++) //Inicializando os vértices
    {
        Spaceship[i].color.assign("WHITE");
        Spaceship[i].id = i + 1;
    }
    for(int i = 0; i < nVertex; i++)
    {
        if(Spaceship[i].color.compare("WHITE") == 0) //Em cada execução é encontrada uma componente conexa
        {
            Spaceship[i].idShip = varNumberShip;
            ShipFind.resize(0); //Inicializando um novo grafo para alocar a nova nave detectada
            ShipFind.pb(Spaceship[i]);
            DFSVisit(i, varNumberShip); //Realizando DFS apartir do vértice i
            ShipsFound.pb(ShipFind); //Adicionando a nave encontrada no conjunto de naves do problema
            varNumberShip++;
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
    for(int i = 0; i < ShipsFound.size(); i++)
    {
        n_edge = number_edge(ShipsFound[i]);
        if(ShipsFound[i].size() == n_edge)
        {
            countShips.conveyors++; //Transportadora -> N.Arestas = N.Vértices
            shipsFoundType.pb(4);
        }
        else if(n_edge == (ShipsFound[i].size() - 1))
        {
            if(is_not_recognition(ShipsFound[i]))
            {
                countShips.frigates++;
                shipsFoundType.pb(2);
            }
            else
            {
                countShips.recognition++;
                shipsFoundType.pb(1);
            }
        }else
        {
            countShips.bombings++;
            shipsFoundType.pb(3);
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
    Spaceship.resize(N);

    //Armazenando o grafo de entrada
    for(int i = 0; i < M; i++)
    {
        cin >> from >> arrival;
        Spaceship[from-1].AdjNodes.pb(arrival-1);
        Spaceship[arrival-1].AdjNodes.pb(from-1);
    }
    DFSNumberOfSpaceships(N); //Aplicando o DFS para identificar o número de naves da tropa inimiga
    identification_ships(); //Detectando cada tipo de nave inimiga

    vvii teleport(ShipsFound.size());
    ii aux;

    //Lendo os teleportes
    for(int i = 0; i < N; i++)
    {
        cin >> from >> arrival;
        aux.first = from - 1;
        aux.second = arrival - 1;
        teleport[Spaceship[from - 1].idShip].pb(aux);
    }
    findDistance(teleport); //Calculando o tempo de vantagem

    //auto finish = chrono::high_resolution_clock::now();
    //chrono::duration<double> elapsed = finish - start;
    //cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}
