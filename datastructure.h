// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <map>
#include <bits/stdc++.h>
#include <unordered_map>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using RouteID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
RouteID const NO_ROUTE = "!!NO_ROUTE!!";
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for time of day in minutes from midnight (i.e., 60*hours + minutes)
using Time = int;

// Return value for cases where color was not found
Time const NO_TIME = std::numeric_limits<Time>::min();

// Type for a duration of time (in minutes)
using Duration = int;

// Return value for cases where Duration is unknown
Duration const NO_DURATION = NO_VALUE;

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



// This is the class you are supposed to implement

class Datastructures
{
public:
public:
    Datastructures();
    ~Datastructures();

    // Tehokkuus: O(1)
    // Palautetaan int-arvo (stops-size).
    int stop_count();

    // Tehokkuus: O(n)
    // Joudutaan tuhoamaan vektorien ja muiden taulukoiden sis??ll??t elementti kerrallaan lineaarisesti.
    // T??t?? funktiota kutsutaan harvoin, joten tehokkuus ei ole t??rke??.
    void clear_all();

    // Tehokkuus: O(1)
    // Palauttaa ainoastaan vektorin, jota ei mahdollisesti ole j??rjestetty.
    std::vector<StopID> all_stops();

    // Tehokkuus: O(1)
    // Etsit????n pys??kki vakioaikaisesti mapista. Lis??ykset vakioaikaisia.
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Tehokkuus: O(1)
    // Etsit????n pys??kki mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Name get_stop_name(StopID id);

    // Tehokkuus: O(1)
    // Etsit????n pys??kki mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Tehokkuus: Kesk. O(n log n) Paras O(1)
    // Suoritetaan Mergesort. Tehd????n vain jos ei ole j??rjestyksess??.
    std::vector<StopID> stops_alphabetically();

    // Tehokkuus: Kesk. O(n log n) Paras O(1)
    // Suoritetaan Mergesort. Tehd????n vain jos ei ole j??rjestyksess??.
    std::vector<StopID> stops_coord_order();

    // Tehokkuus: O(1)
    // Palautetaan tallessa oleva pienin (koordinaatti) stoppi.
    StopID min_coord();

    // Tehokkuus: O(1)
    // Palautetaan tallessa oleva isoin (koordinaatti) stoppi.
    StopID max_coord();

    // Tehokkuus: O(n)
    // Etsit????n pys??kit taulukosta lineaarisesti.
    // T??t?? funktiota kutsutaan harvoin, joten tehokkuus ei ole t??rke??.
    std::vector<StopID> find_stops(Name const& name);

    // Tehokkuus: O(1)
    // Etsit????n pys??kki mapista, jonka tehokkuus on vakio.
    bool change_stop_name(StopID id, Name const& newname);

    // Tehokkuus: O(1)
    // Etsit????n pys??kki mapista, jonka tehokkuus on vakio.
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Tehokkuus: O(1)
    // Etsit????n alue vakioaikaisesti mapista. Lis??ykset vakioaikaisia.
    bool add_region(RegionID id, Name const& name);

    // Tehokkuus: O(1)
    // Etsit????n alue mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Name get_region_name(RegionID id);

    // Tehokkuus: O(1)
    // Palauttaa vektorin.
    std::vector<RegionID> all_regions();

    // Tehokkuus: (1)
    // Etsit????n pys??kki ja alue mapeista vakioaikaisesti.
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Tehokkuus: O(1)
    // Alueet etsit????n vakioaikaisesti mapeista.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Tehokkuus: O(n)
    // Etsit????n pys??kki vakioaikaisesti mapista. Voidaan joutua tyhjent??m????n vektori.
    // Sitten rekursiivisesti haetaan stopin alueet. Etsint?? ei ole tarpeen, sill??
    // alueiden paikat taulukossa on tallennettu erikseen.
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Tehokkuus: O(1)
    // Ei tehd?? mit????n.
    void creation_finished();

    // Tehokkuus: O(n)
    // Alue etsit????n vakioaikaisesti mapista.
    // K??yd????n alue ja kaikki sen ali-alueet ja stopit vain kerran l??pi.
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Tehokkuus: O(n)
    // Pys??kki etsit????n vakioaikaisesti mapista. K??yd????n jokaisen pys??kin et??isyys
    // lineaarisesti l??pi taulukosta. Otetaan viisi ensimm??ist?? pys??kki??.
    std::vector<StopID> stops_closest_to(StopID id);

    // Tehokkuus: O(n)
    // Pys??kki etsit????n vakioaikaisesti mapista. Sen alueen pys??kin k??yd????nn lineaarisesti l??pi.
    // My??s stops_indexes p??ivitet????n, mik?? on lineaarista.
    // T??t?? funktiota kutsutaan harvoin, joten tehokkuus ei ole t??rke??.
    bool remove_stop(StopID id);

    // Tehokkuus: O(n)
    // Molemmat pys??kit etsit????n vakioaikaisesti.
    // K??yd????n molempien pys??kkien alue-reitit yl??sp??in ja hashilla etsit????n yhteinen alue.
    RegionID stops_common_region(StopID id1, StopID id2);

    // Phase 2 operations

    // Tehokkuus: O(1)
    // Palauttaa tallennetun vektorin.
    std::vector<RouteID> all_routes();

    // Tehokkuus: O(n)
    // Lis??t????n vakioaikaisesti reitti-vektoriin.
    // Merkit????n jokaiselle reitin pys??keille tieto, mihin reittiin ne kuuluvat lineaarisesti.
    bool add_route(RouteID id, std::vector<StopID> stops_table);

    // Tehokkuus: O(n)
    // K??yd????n l??pi kaikki reitit, jota pys??kill?? on.
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Tehokkuus: O(1)
    // Palautetaan vektori.
    std::vector<StopID> route_stops(RouteID id);

    // Tehokkuus: O(n)
    // Vektorin tyhjennys on lineaarista.
    void clear_routes();

    // Tehokkuus: O(p+e), jossa p on pys??kkien m????r?? ja e tiet niiden v??liss??.
    // K??ytet????n BFS-algoritmia.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_any(StopID fromstop, StopID tostop);

//    // Non-compulsory operations

    // Tehokkuus: O(p+e), jossa p on pys??kkien m????r?? ja e tiet niiden v??liss??.
    // K??ytet????n BFS-algoritmia.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_least_stops(StopID fromstop, StopID tostop);

    // Tehokkuus: O(1)
    // Ei saatu toimimaan.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_with_cycle(StopID fromstop);

    // Tehokkuus: O(v^2)
    // Dijakstran-algoritmin toteutus, jossa v on pys??kkien m????r??.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_shortest_distance(StopID fromstop, StopID tostop);

    // Tehokkuus: O(n)
    // Etsit????n find-funktiolla reitti??.
    bool add_trip(RouteID routeid, const std::vector<Time> &stop_times);

    // Tehokkuus: O(n)
    // K??yd????n l??vitse reittej?? ja pys??kkej??.
    std::vector<std::pair<Time, Duration> > route_times_from(RouteID routeid, StopID stopid);

    // Tehokkuus: O(1)
    // Ei saatu toimimaan.
    std::vector<std::tuple<StopID, RouteID, Time>> journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime);

    // Tehokkuus: O(1)
    // Ei saatu toimimaan.
    void add_walking_connections(); // Note! This method is completely optional, and not part of any testing

private:

    // Stoppien tiedot talletettu yhteen struktiin.

    struct stop
    {
        StopID id;
        Name name;
        Coord coordinates;
        RegionID region_id;     // Alueen, johon stoppi kuuluu, id.
        bool has_region;
        int region_index;       // Miss?? alue piilee taulukossa.
        std::vector<RouteID> stop_routes;
        std::unordered_map<RouteID,std::pair<int,int>> next_stop;
    };

    // Muuttujia stoppien k??sittelyyn.

    std::vector<stop> stops;             // Taulukko, jossa stoppeja pidet????n.
    std::vector<stop> stops_temp;        // V??liaikainen stoppi j??rjest??miseen.
    std::vector<StopID> stopIDs;         // Pidet????n tallessa stoppien id:t, josta ne on helppo palauttaa.
    std::vector<StopID> stopIDs_name;    // Stoppien id:t aakkosj??rjestyksess??. V??hent???? turhaa sorttausta.
    std::vector<StopID> stopIDs_coord;   // Stoppien id:t koordinaattij??rjestyksess??.
    bool stops_sorted_name;              // Onko stopit j??rjestetty aakkosten mukaan.
    bool stops_sorted_coord;             // Onko stopit j??rjestetty koordinaattien mukaan.
    int stops_size;
    std::unordered_map<StopID,int> stop_indexes;     // Stoppien sijainnit vektorissa mapissa. T??m??, jotta saadaan nimien haku vakioksi.

    // N??ill?? muuttujilla pidet????n kirjaa pienimm??st?? ja isoimmasta koordinaateista.
    // V??hennet????n turhaa etsint????.

    Coord min_coord_stop;
    StopID min_stop_stop;
    Coord max_coord_stop;
    StopID max_stop_stop;

    // Alueen strukti, jossa on sen tietoja.

    struct region
    {
        RegionID id;
        Name name;
        bool has_parent;
        int parent_index;
        std::vector<int> subregions_index;     // Alueen alia-alueiden sijainnit alue-taulukossa.
        std::vector<int> region_stops_index;   // Alueen stoppien sijainnit stoppi-taulukossa.
    };

    // Muuttujia alueiden k??sittelyyn.

    std::vector<region> regions;                   // Taulukko, jossa alueita pidet????n.
    std::vector<RegionID> regionIDs;               // Taulukko, jossa pidet????n alueiden id:t tallessa, josta ne helposti voidaan kysy??.
    std::vector<RegionID> stop_regions_results;    // K??ytet????n funktiossa stop_regions ja sit?? k??ytet????n tallettamaan sen tulokset (stopin alueet).
    std::vector<StopID> region_stops_results;      // K??ytet????n tilanteissa, jossa halutaan tiet???? alueen ali-alueiden stopit.
    std::vector<RegionID> common_region_path;      // Talletaan kahden alueen polku yl??sp??in. Funktio: stops_common_region.
    int regions_size;
    std::unordered_map<RegionID,int> region_indexes;     // Alueiden sijainnissa vektorissa mapissa. T??m??, jotta saadaan nimien haku vakioksi.


    // Muuttujia reittien k??sittelyyn

    std::unordered_map<RegionID,std::vector<StopID>> routes;   // Reiteille ei tarvitse k??ytt???? tietueita, kun vain 2 elementti??.
    std::vector<RouteID> routeIDs;                              // Jotta voidaan helposti palauttaa.
    int routes_size;

    // Apumuuttujia algoritmeissa.

    std::unordered_map<StopID,int> distances;
    std::vector<std::tuple<StopID, RouteID,Distance>> dijkstra_results;

    // Matkan strukti

    struct trip {
        RouteID routetrip_id;
        std::vector<Time> stop_times;
    };

    // Muuttujia matkojen k??sittelyyn.

    std::vector<trip> trips;    // Matkat muistiin.
    int trips_size = 0;

    // Omia funktioita, joista kuvaus datastructures.cc-tiedoston puolella.

    void update_IDs(std::string mode);
    void merge(int p, int q, int r, std::string mode);
    void mergesort(int p, int r, std::string mode);
    bool comparison(stop stop1, stop stop2, std::string mode);
    void region_path(int index);
    void set_min_max_stop(stop newstop);
    void find_region_stops(int region_index);
    bool find_subregions(int region_index, RegionID id);
    int distance_between_stops(int stop1, int stop2);
    std::vector<std::tuple<StopID, int, RouteID, Distance>> bfs(StopID src, StopID target);
    std::vector<std::tuple<StopID, int, RouteID, Distance>> dfs(StopID src);
    std::vector<std::tuple<StopID, RouteID, Distance>> route_tracker(std::vector<std::tuple<StopID, int, RouteID, Distance>> route_map);
    StopID minDistance(std::unordered_map<StopID, int> dist, std::unordered_map<StopID, bool> sptSet);
    std::vector<std::tuple<StopID, RouteID, Distance> > dijkstra(StopID src, StopID target);
    std::vector<std::tuple<StopID, RouteID,Distance>> create_path(std::unordered_map<StopID, StopID> parent, std::unordered_map<StopID, int> dist, StopID src, StopID target, std::unordered_map<StopID, RouteID> routes);
};

#endif // DATASTRUCTURES_HH
