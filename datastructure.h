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
    // Joudutaan tuhoamaan vektorien ja muiden taulukoiden sisällöt elementti kerrallaan lineaarisesti.
    // Tätä funktiota kutsutaan harvoin, joten tehokkuus ei ole tärkeä.
    void clear_all();

    // Tehokkuus: O(1)
    // Palauttaa ainoastaan vektorin, jota ei mahdollisesti ole järjestetty.
    std::vector<StopID> all_stops();

    // Tehokkuus: O(1)
    // Etsitään pysäkki vakioaikaisesti mapista. Lisäykset vakioaikaisia.
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Tehokkuus: O(1)
    // Etsitään pysäkki mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Name get_stop_name(StopID id);

    // Tehokkuus: O(1)
    // Etsitään pysäkki mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Tehokkuus: Kesk. O(n log n) Paras O(1)
    // Suoritetaan Mergesort. Tehdään vain jos ei ole järjestyksessä.
    std::vector<StopID> stops_alphabetically();

    // Tehokkuus: Kesk. O(n log n) Paras O(1)
    // Suoritetaan Mergesort. Tehdään vain jos ei ole järjestyksessä.
    std::vector<StopID> stops_coord_order();

    // Tehokkuus: O(1)
    // Palautetaan tallessa oleva pienin (koordinaatti) stoppi.
    StopID min_coord();

    // Tehokkuus: O(1)
    // Palautetaan tallessa oleva isoin (koordinaatti) stoppi.
    StopID max_coord();

    // Tehokkuus: O(n)
    // Etsitään pysäkit taulukosta lineaarisesti.
    // Tätä funktiota kutsutaan harvoin, joten tehokkuus ei ole tärkeä.
    std::vector<StopID> find_stops(Name const& name);

    // Tehokkuus: O(1)
    // Etsitään pysäkki mapista, jonka tehokkuus on vakio.
    bool change_stop_name(StopID id, Name const& newname);

    // Tehokkuus: O(1)
    // Etsitään pysäkki mapista, jonka tehokkuus on vakio.
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Tehokkuus: O(1)
    // Etsitään alue vakioaikaisesti mapista. Lisäykset vakioaikaisia.
    bool add_region(RegionID id, Name const& name);

    // Tehokkuus: O(1)
    // Etsitään alue mapista id:n perusteella, jossa operaatio on tehokkuudeltaan O(1).
    Name get_region_name(RegionID id);

    // Tehokkuus: O(1)
    // Palauttaa vektorin.
    std::vector<RegionID> all_regions();

    // Tehokkuus: (1)
    // Etsitään pysäkki ja alue mapeista vakioaikaisesti.
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Tehokkuus: O(1)
    // Alueet etsitään vakioaikaisesti mapeista.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Tehokkuus: O(n)
    // Etsitään pysäkki vakioaikaisesti mapista. Voidaan joutua tyhjentämään vektori.
    // Sitten rekursiivisesti haetaan stopin alueet. Etsintä ei ole tarpeen, sillä
    // alueiden paikat taulukossa on tallennettu erikseen.
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Tehokkuus: O(1)
    // Ei tehdä mitään.
    void creation_finished();

    // Tehokkuus: O(n)
    // Alue etsitään vakioaikaisesti mapista.
    // Käydään alue ja kaikki sen ali-alueet ja stopit vain kerran läpi.
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Tehokkuus: O(n)
    // Pysäkki etsitään vakioaikaisesti mapista. Käydään jokaisen pysäkin etäisyys
    // lineaarisesti läpi taulukosta. Otetaan viisi ensimmäistä pysäkkiä.
    std::vector<StopID> stops_closest_to(StopID id);

    // Tehokkuus: O(n)
    // Pysäkki etsitään vakioaikaisesti mapista. Sen alueen pysäkin käydäänn lineaarisesti läpi.
    // Myös stops_indexes päivitetään, mikä on lineaarista.
    // Tätä funktiota kutsutaan harvoin, joten tehokkuus ei ole tärkeä.
    bool remove_stop(StopID id);

    // Tehokkuus: O(n)
    // Molemmat pysäkit etsitään vakioaikaisesti.
    // Käydään molempien pysäkkien alue-reitit ylöspäin ja hashilla etsitään yhteinen alue.
    RegionID stops_common_region(StopID id1, StopID id2);

    // Phase 2 operations

    // Tehokkuus: O(1)
    // Palauttaa tallennetun vektorin.
    std::vector<RouteID> all_routes();

    // Tehokkuus: O(n)
    // Lisätään vakioaikaisesti reitti-vektoriin.
    // Merkitään jokaiselle reitin pysäkeille tieto, mihin reittiin ne kuuluvat lineaarisesti.
    bool add_route(RouteID id, std::vector<StopID> stops_table);

    // Tehokkuus: O(n)
    // Käydään läpi kaikki reitit, jota pysäkillä on.
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Tehokkuus: O(1)
    // Palautetaan vektori.
    std::vector<StopID> route_stops(RouteID id);

    // Tehokkuus: O(n)
    // Vektorin tyhjennys on lineaarista.
    void clear_routes();

    // Tehokkuus: O(p+e), jossa p on pysäkkien määrä ja e tiet niiden välissä.
    // Käytetään BFS-algoritmia.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_any(StopID fromstop, StopID tostop);

//    // Non-compulsory operations

    // Tehokkuus: O(p+e), jossa p on pysäkkien määrä ja e tiet niiden välissä.
    // Käytetään BFS-algoritmia.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_least_stops(StopID fromstop, StopID tostop);

    // Tehokkuus: O(1)
    // Ei saatu toimimaan.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_with_cycle(StopID fromstop);

    // Tehokkuus: O(v^2)
    // Dijakstran-algoritmin toteutus, jossa v on pysäkkien määrä.
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_shortest_distance(StopID fromstop, StopID tostop);

    // Tehokkuus: O(n)
    // Etsitään find-funktiolla reittiä.
    bool add_trip(RouteID routeid, const std::vector<Time> &stop_times);

    // Tehokkuus: O(n)
    // Käydään lävitse reittejä ja pysäkkejä.
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
        int region_index;       // Missä alue piilee taulukossa.
        std::vector<RouteID> stop_routes;
        std::unordered_map<RouteID,std::pair<int,int>> next_stop;
    };

    // Muuttujia stoppien käsittelyyn.

    std::vector<stop> stops;             // Taulukko, jossa stoppeja pidetään.
    std::vector<stop> stops_temp;        // Väliaikainen stoppi järjestämiseen.
    std::vector<StopID> stopIDs;         // Pidetään tallessa stoppien id:t, josta ne on helppo palauttaa.
    std::vector<StopID> stopIDs_name;    // Stoppien id:t aakkosjärjestyksessä. Vähentää turhaa sorttausta.
    std::vector<StopID> stopIDs_coord;   // Stoppien id:t koordinaattijärjestyksessä.
    bool stops_sorted_name;              // Onko stopit järjestetty aakkosten mukaan.
    bool stops_sorted_coord;             // Onko stopit järjestetty koordinaattien mukaan.
    int stops_size;
    std::unordered_map<StopID,int> stop_indexes;     // Stoppien sijainnit vektorissa mapissa. Tämä, jotta saadaan nimien haku vakioksi.

    // Näillä muuttujilla pidetään kirjaa pienimmästä ja isoimmasta koordinaateista.
    // Vähennetään turhaa etsintää.

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

    // Muuttujia alueiden käsittelyyn.

    std::vector<region> regions;                   // Taulukko, jossa alueita pidetään.
    std::vector<RegionID> regionIDs;               // Taulukko, jossa pidetään alueiden id:t tallessa, josta ne helposti voidaan kysyä.
    std::vector<RegionID> stop_regions_results;    // Käytetään funktiossa stop_regions ja sitä käytetään tallettamaan sen tulokset (stopin alueet).
    std::vector<StopID> region_stops_results;      // Käytetään tilanteissa, jossa halutaan tietää alueen ali-alueiden stopit.
    std::vector<RegionID> common_region_path;      // Talletaan kahden alueen polku ylöspäin. Funktio: stops_common_region.
    int regions_size;
    std::unordered_map<RegionID,int> region_indexes;     // Alueiden sijainnissa vektorissa mapissa. Tämä, jotta saadaan nimien haku vakioksi.


    // Muuttujia reittien käsittelyyn

    std::unordered_map<RegionID,std::vector<StopID>> routes;   // Reiteille ei tarvitse käyttää tietueita, kun vain 2 elementtiä.
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

    // Muuttujia matkojen käsittelyyn.

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
