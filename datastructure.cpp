// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    stops_size = 0;
    stops_sorted_name = false;
    stops_sorted_coord = false;
    regions_size = 0;
    routes_size = 0;
}

Datastructures::~Datastructures()
{
    clear_all();
}

int Datastructures::stop_count()
{
    return stops_size;     // Palauttaa muuttujan, jossa talletetaan stoppi-taulukon kokoa.
}

void Datastructures::clear_all()
{
    // Tyhjennetään kaikki taulukot tarvittaessa ja asetetaan tietyt muuttujat arvoihin nolla.

    stops_size = 0;
    if(stops.size() != 0) { stops.clear(); }
    if(stops_temp.size() != 0) { stops_temp.clear(); }
    if(stopIDs.size() != 0) { stopIDs.clear(); }
    if(stopIDs_coord.size() != 0) { stopIDs_coord.clear(); }
    if(stopIDs_name.size() != 0) { stopIDs_name.clear(); }
    if(stop_indexes.size() != 0) { stop_indexes.clear(); }
    stops_sorted_name = false;
    stops_sorted_coord = false;

    regions_size = 0;
    if(regions.size() != 0) { regions.clear(); }
    if(regionIDs.size() != 0) { regionIDs.clear(); }
    if(stop_regions_results.size() != 0) { stop_regions_results.clear(); }
    if(region_stops_results.size() != 0) { region_stops_results.clear(); }
    if(common_region_path.size() != 0) { common_region_path.clear(); }
    if(region_indexes.size() != 0) { region_indexes.clear(); }

    routes_size = 0;
    if(routes.size() == 0) { routes.clear(); }
}

std::vector<StopID> Datastructures::all_stops()
{
    return stopIDs;
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    // Etsitään ensin pysäkki mapista sen id:n perusteella johtuen operaation tehokkuudesta ja
    // jotta nähdään onko pysäkki jo olemassa.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        return false;
    }

    // Luodaan stoppi ja laitetaan se talteen.

    stop newStop;
    newStop.id = id;
    newStop.name = name;
    newStop.coordinates = xy;
    newStop.has_region = false;
    stops.push_back(newStop);
    stopIDs.push_back(id);
    stop_indexes[id] = stops_size;
    stops_sorted_name = false;
    stops_sorted_coord = false;
    ++stops_size;

    // Tsekataan onko stopin koordinaatit pienin tai suurin olemassa-olevista.
    // Täten ei tarvitse etsiä jälkikäteen.

    set_min_max_stop(newStop);

    return true;
}

Name Datastructures::get_stop_name(StopID id)
{
   if(stops_size == 0) { return NO_NAME; }   // Jos ei ole pysäkkejä.

   // Etsitään pysäkki sen id:n perusteella mapista.

   std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
   if(got != stop_indexes.end()){
       return stops[got->second].name;
   }
   return NO_NAME;
}

Coord Datastructures::get_stop_coord(StopID id)
{
    if(stops_size == 0) { return NO_COORD; } // Jos ei ole pysäkkejä.

    // Etsitään pysäkki sen id:n perusteella mapista.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        return stops[got->second].coordinates;
    }
     return NO_COORD;
}

std::vector<StopID> Datastructures::stops_alphabetically()
{
    if(!stops_sorted_name && stops.size() != 0) {     // Jos pysäkit eivät ole jo järjestyksessä.
        stops_temp = stops;                    // Käytetään erillistä temp-taulukkoa, jotta stopit pysyvät tietyssä omassa järjestyksessä.
        mergesort(0, stops_size-1, "name");    // Suoritetaan mergesortti järjestämiselle.
        update_IDs("name");                    // Päivitetään järjestetty id-taulukko, joka sitten palautetaan.
        stops_sorted_name = true;
    }
    return stopIDs_name;
}

std::vector<StopID> Datastructures::stops_coord_order()
{
    if(!stops_sorted_coord && stops.size() != 0) {     // Jos pysäkit eivät ole jo järjestyksessä.
        stops_temp = stops;                             // Käytetään erillisestä temp-taulukkoa, jotta stopit pysyvät tietyssä omassa järjestyksessä.
        mergesort(0, stops_size-1, "coordinates");      // Suoritetaan mergesortti järjestämiselle.
        update_IDs("coord");                            // Päivitetään järjestetty id-taulukko, joka sitten palautetaan.
        stops_sorted_coord = true;
    }
    return stopIDs_coord;
}

StopID Datastructures::min_coord()
{
    if(stops_size == 0){ return NO_STOP;}     // Jos ei ole yhtään pysäkkiä.
    return min_stop_stop;                     // Palautetaan tallessa oleva pysäkki.
}

StopID Datastructures::max_coord()
{
    if(stops_size == 0){ return NO_STOP; }     // Jos ei ole yhtään pysäkkiä.
    return max_stop_stop;                      // Palautetaan tallessa oleva pysäkki.
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    std::vector<StopID> found_stops;

    if(stops_size == 0){ return found_stops; }     // Jos pysäkkejä ei ole ole.

    // Laitetaan talteen kaikki löydetyt pysäkit.

    for(int i = 0; i < stops_size; ++i){
        if(stops[i].name == name){
            found_stops.push_back(stops[i].id);
        }
    }
    return found_stops;
}

bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    if(stops_size == 0){ return false; }       // Jos pysäkkejä  ei ole.

    int stop_index = 0;

    // Etsitään pysäkki sen id:n perusteella mapista.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return false; } // Ei löytynyt.

    // Vaihdetaan sitten nimi.

    stops[stop_index].name = newname;
    stops_sorted_name = false;     // Nimet eivät ole enään aakkosjärjestyksessä.

    return true;
}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    if(stops_size == 0){ return false; }        // Jos pysäkkejä ei ole.

    int stop_index = 0;

    // Etsitään pysäkki sen id:n perusteella mapista.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return false; }   // Ei löytynyt.

    // Vaihdetaan sitten koordinaatit.

    stops[stop_index].coordinates = newcoord;
    stops_sorted_coord = false;     // Koordinaatit eivät ole enään järjestyksessä.

    return true;
}

bool Datastructures::add_region(RegionID id, const Name &name)
{
    // Etsitään ensin alue mapista sen id:n perusteella johtuen operaation tehokkuudesta ja
    // jotta nähdään onko alue jo olemassa.

    std::unordered_map<RegionID,int>::const_iterator got = region_indexes.find(id);
    if(got != region_indexes.end()){
        return false;
    }

    // Luodaan alue.

    region newRegion;
    newRegion.id = id;
    newRegion.name = name;
    newRegion.has_parent = false;
    regions.push_back(newRegion);
    regionIDs.push_back(id);
    region_indexes[id] = regions_size;
    ++regions_size;
    return true;
}

Name Datastructures::get_region_name(RegionID id)
{
    if(regions_size == 0) { return NO_NAME; }    // Jos aluetta ei ole olemassa.

    // Etsitään alue vakioaikaisesti mapista.

    std::unordered_map<RegionID,int>::const_iterator got = region_indexes.find(id);
    if(got != region_indexes.end()){
        return regions[got->second].name;
    }
    return NO_NAME;
}

std::vector<RegionID> Datastructures::all_regions()
{
    return regionIDs;                               // Palautetaan tallessa oleva alue-ID-taulukko.
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    if(stops_size == 0 || regions_size == 0) { return false; }  // Jos alueita tai stoppeja ei ole.

    int stop_index = 0;
    int region_index = 0;
    bool stop_found = false;

    // Etsitään ensin pysäkki id:n avulla vakioaikaisesti.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
        stop_found = true;
    }
    if(stops[stop_index].has_region || !stop_found) { return false; } // Jos pysäkki kuuluu jo alueeseen tai sitä ei ole.

    // Etsitään ensin alue id:n avulla vakioaikaisesti.

    std::unordered_map<RegionID,int>::const_iterator parentgot = region_indexes.find(parentid);
    if(parentgot != region_indexes.end()){
        region_index = parentgot->second;
    }
    else { return false; } // Jos aluetta ei ole.

    // Lisätään sitten pysäkki ja alue toisiinsa.

    stops[stop_index].region_id = parentid;
    stops[stop_index].region_index = region_index;
    stops[stop_index].has_region = true;
    regions[region_index].region_stops_index.push_back(stop_index);
    return true;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
     if(regions_size == 0) { return false; }    // Jos alueita ei ole.

    int subregion_index = 0;
    int parentregion_index = 0;

    // Etsitään molemmat alueet vakioaikaisesti mapeista.

    std::unordered_map<RegionID,int>::const_iterator got = region_indexes.find(id);
    if(got != region_indexes.end()){
        subregion_index = got->second;
    }
    else { return false; }

    std::unordered_map<RegionID,int>::const_iterator parentgot = region_indexes.find(parentid);
    if(parentgot != region_indexes.end()){
        parentregion_index = parentgot->second;
    }
    else { return false; }

    if(regions[subregion_index].has_parent){   // Jos ali-alueella on jo ylempi alue.
        return false;
    }

    // Tarkistetaan kuuluuko tuleva yli-alue jo tulevan ali-alueen ali-alueisiin, jottei tule syklejä.
    // Tätä ei tosin enään tarvitse huomioida työohjeen päivityksen jälkeen.

    //  if(regions[subregion_index].subregions_index.size() != 0){
    //      if(!find_subregions(subregion_index,regions[parentregion_index].id)){
    //         return false;
    //      }
    //  }

    // Lisätään alueet toisiinsa.

    regions[subregion_index].parent_index = parentregion_index;
    regions[subregion_index].has_parent = true;
    regions[parentregion_index].subregions_index.push_back(subregion_index);

    return true;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
     if(stops_size == 0) { return {NO_REGION}; }   // Jos pysäkkejä tai alueita ei ole.

    int stop_index = 0;

    if(stop_regions_results.size() != 0) { stop_regions_results.clear(); }

    if(regions_size == 0) { return stop_regions_results; } // Jos alueita ei ole.

    // Etsitään pysäkki sen id:n perusteella mapista vakioaikaisesti.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return {NO_REGION}; }   // Pysäkkiä ei löydetty.

    if(!stops[stop_index].has_region) { return stop_regions_results; } // Pysäkki ei kuuluu mihinkään alueeseen.

    // Aletaan rekursiivisesti menemään pysäkin alueita ylöspäin. Mennään esim. Pysäkki->Alue->Yli-alue->Yliyli-alue->...
    // Laitetaan nämä muistiin ja palautetaan ne.

    region_path(stops[stop_index].region_index);

    return stop_regions_results;
}

void Datastructures::creation_finished()
{
    // Ei hyödynnetty.
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{
    if(regions_size == 0) { return {NO_COORD, NO_COORD}; }   // Jos alueita ei ole.

    int region_index = 0;
    Coord min_coord_region;
    Coord max_coord_region;
    double min_value = 0;
    double max_value = 0;
    double comp_value = 0;
    region_stops_results.clear();

    // Etsitään ensin alue sen id:n perusteella vakioaikaisesti mapista.

    std::unordered_map<RegionID,int>::const_iterator got = region_indexes.find(id);
    if(got != region_indexes.end()){
        region_index = got->second;
    }
    else { return {NO_COORD, NO_COORD}; } // Jos aluetta ei ole.

    // Tässä mennään taso kerrallaan käsittelemällä jokainen alue ja niiden ali-alueet
    // ja ottamalla niiden stopit talteen. Toteutus on tehty rekursiivisesti.

    find_region_stops(region_index);

    if(region_stops_results.size() == 0) { return {NO_COORD, NO_COORD}; } // Ei löydetty yhtään pysäkkiä.

    // Laitetaan ensin löydetyistä stopeista ensimmäinen aluksi vertailukohteeksi.

    min_coord_region = stops[region_stops_results[0]].coordinates;
    max_coord_region = stops[region_stops_results[0]].coordinates;

    // Käydään löydetyt pysäkit lävitse ja etsitään pienin ja suurin koordinaatin omaava.

    for(size_t j = 1; j < region_stops_results.size(); ++j){
        min_value = sqrt(pow(min_coord_region.x,2.0)+pow(min_coord_region.y,2.0));
        max_value = sqrt(pow(max_coord_region.x,2.0)+pow(max_coord_region.y,2.0));
        comp_value = sqrt(pow(stops[region_stops_results[j]].coordinates.x,2.0)+pow(stops[region_stops_results[j]].coordinates.y,2.0));

        if(comp_value < min_value){
            min_coord_region = stops[region_stops_results[j]].coordinates;
        }
        if(comp_value > max_value){
            max_coord_region = stops[region_stops_results[j]].coordinates;
        }
    }

    return std::make_pair(min_coord_region,max_coord_region);
}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    // Apu taulukoita. Map-taulukot tarjoavat nopean ratkaisun pitää elementtejä järjestyksessä.

    std::vector<StopID> nearest_stops;
    std::multimap<double,int> distances;
    std::multimap<double,int>::iterator itr;

    double distance = 0;
    int stop_index = 0;
    int element_amount = 0;

    if(stops_size == 0) { return nearest_stops; }  // Jos pysäkkejä ei ole.

    // Etsitään pysäkki id:n avulla vakioaikaisesti mapilla.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return {NO_STOP}; } // Jos pysäkkiä ei löytynyt.

    // Lasketaan jokaisen pysäkin etäisyys löydettyyn pysäkkiin.

    for(int j = 0; j < stops_size; ++j){
        distance = sqrt(pow(stops[stop_index].coordinates.x-stops[j].coordinates.x,2.0)+pow(stops[stop_index].coordinates.y-stops[j].coordinates.y,2.0));
        if(stops[j].id != stops[stop_index].id){                   // Ei saa olla sama stoppi, johon vertaillaan.
            distances.insert(std::pair<double,int>(distance,j));   // Lisätään etäisyydet mappiin.
        }
    }

    // Lisätään enintään viisi elementtiä palautettavaan taulukkoon.
    // Täytyy palauttaa vektorina.

    for(itr = distances.begin(); itr != distances.end(); ++itr){
        if(element_amount < 5){
            nearest_stops.push_back(stops[itr->second].id);
        }
        else {
            break;
        }
        ++element_amount;
    }

    return nearest_stops;
}

bool Datastructures::remove_stop(StopID id)
{
    if(stops_size == 0) { return false; }  // Jos pysäkkejä ei ole.

    int stop_index = 0;

    // Etsitään pysäkki id:n avulla vakioaikaisesti mapilla.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(id);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return false; } // Jos pysäkkiä ei löytynyt.

    // Jos pysäkki kuului alueeseen, poistetaan pysäkin id alueen listasta.

    if(stops[stop_index].has_region){
       for(size_t j = 0; j < regions[stops[stop_index].region_index].region_stops_index.size(); ++j){
           if(regions[stops[stop_index].region_index].region_stops_index[j] == stop_index){
                regions[stops[stop_index].region_index].region_stops_index.erase(regions[stops[stop_index].region_index].region_stops_index.begin() + j);
                break;
           }
       }
    }

    // Poistetaan pysäkki ja sen id.

    stops.erase(stops.begin() + stop_index);
    stopIDs.erase(stopIDs.begin() + stop_index);
    stops_sorted_name = false;
    stops_sorted_coord = false;
    --stops_size;

    // Päivitetään stop_indexes.

    stop_indexes.clear();
    for(int i = 0; i < stops_size; ++i) {
        stop_indexes[stops[i].id] = i;
    }
    return true;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    int stop_index1 = 0;
    int stop_index2 = 0;
    int min = -1;
    stop_regions_results.clear();
    std::set<RegionID> myset;  // Hash-taulukko, joka on nopea etsinnässä.

    // Etsitään molemmat pysäkit vakioaikaisesti.

    std::unordered_map<StopID,int>::const_iterator got1 = stop_indexes.find(id1);
    if(got1 != stop_indexes.end()){
        stop_index1 = got1->second;
    }
    else { return NO_REGION; } // Jos pysäkkiä ei löytynyt.

    std::unordered_map<StopID,int>::const_iterator got2 = stop_indexes.find(id2);
    if(got2 != stop_indexes.end()){
        stop_index2 = got2->second;
    }
    else { return NO_REGION; } // Jos pysäkkiä ei löytynyt.

    // Jos jommalla kummalla pysäkeistä ei ole aluetta.

    if(!stops[stop_index1].has_region || !stops[stop_index2].has_region){
        return NO_REGION;
    }

    // Haetaan molemman pysäkit alueet ylöspäin. Esim. pysäkki->alue->yli-alue->...
    // Nämä alueet (reitit, niiden id:t) sitten talletetaan muistiin samaan taulukkoon.

    region_path(stops[stop_index1].region_index);
    region_path(stops[stop_index2].region_index);

    // Etsitään ensimmäinen pari taulukosta, joka on yhteisen alueen id, ja se on
    // myös "alin", kun se tulee vastaan ekana.
    // Hash-taulukolla etsintä on nopeaa.

    for(size_t j = 0; j < stop_regions_results.size(); ++j){
        if(myset.find(stop_regions_results[j]) != myset.end()){
            min = j;
            break;
        }
        else  {
            myset.insert(stop_regions_results[j]);
        }
    }

    if(min == -1){ return NO_REGION; }  // Jos yhteistä aluetta ei löytynyt.
    return stop_regions_results[min];
}

// Toisen vaiheen funktiot

std::vector<RouteID> Datastructures::all_routes()
{
    return routeIDs;
}

bool Datastructures::add_route(RouteID id, std::vector<StopID> stops_table)
{
    int previous_stop_index = 0;

    // Tarkistetaan onko reittiä olemassa.

    std::unordered_map<RouteID,std::vector<StopID>>::const_iterator got = routes.find(id);
    if(got != routes.end()){
        return false;
    }

    // Lisätään reitin pysäkkeihin tieto mihin reittiin ne kuuluvat ja etäisyys viereiseen pysäkkiin.

    for(size_t i = 0; i < stops_table.size(); ++i) {
        std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(stops_table[i]);
        if(got != stop_indexes.end()){
            stops[got->second].stop_routes.push_back(id);
            if(i > 0) {
                stops[previous_stop_index].next_stop[id].first = got->second;   // Reitti
                stops[previous_stop_index].next_stop[id].second = distance_between_stops(previous_stop_index, got->second);
            }
            previous_stop_index = got->second;
       }
    }

    routes[id] = stops_table;
    routeIDs.push_back(id);
    ++routes_size;

    return true;
}

std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID stopid)
{
    int stop_index = 0;

    // Tarkistetaan onko pysäkkiä olemassa.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(stopid);
    if(got != stop_indexes.end()){
        stop_index = got->second;
    }
    else { return {{NO_ROUTE, NO_STOP}}; }

    std::vector<std::pair<RouteID, StopID>> routes_from_results;

    // Käy jokaisen pysäkin reitin lävitse ja laittaa talteen myös, mikä on seuraava pysäkki.

    std::unordered_map<RouteID, std::pair<int,int>>::iterator it = stops[stop_index].next_stop.begin();
    while(it != stops[stop_index].next_stop.end()){
        routes_from_results.push_back(std::make_pair(it->first,stops[it->second.first].id));
        ++it;
    }

    return routes_from_results;
}

std::vector<StopID> Datastructures::route_stops(RouteID id)
{
    // Etsitään reitti ja palautetaan sen pysäkit.

    std::unordered_map<RouteID,std::vector<StopID>>::const_iterator got = routes.find(id);
    if(got != routes.end()){
        return routes.at(got->first);
    }

    return {NO_STOP};
}

void Datastructures::clear_routes()
{
    routes.clear();
    routes_size = 0;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_any(StopID fromstop, StopID tostop)
{
    // Palautetaan matka, jossa on vähiten pysäkkejä.

    return journey_least_stops(fromstop,tostop);
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_least_stops(StopID fromstop, StopID tostop)
{
    // Tarkistetaan onko pysäkkejä olemassa.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(fromstop);
    if(got == stop_indexes.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    got = stop_indexes.find(tostop);
    if(got == stop_indexes.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }

    // Suoritetaan bfs-algoritmi ja route_tracker-funktio käytetään reitin tallennukseen palautusta varten.

    return route_tracker(bfs(fromstop, tostop));
}

// En saanut tätä toimimaan.

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_with_cycle(StopID fromstop)
{
    int stop_index = 0;

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(fromstop);
    if(got == stop_indexes.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    else{ stop_index = got->second; }


    return route_tracker(bfs(fromstop, NO_STOP));
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_shortest_distance(StopID fromstop, StopID tostop)
{
    // Tarkistetaan onko pysäkkejä olemassa.

    std::unordered_map<StopID,int>::const_iterator got = stop_indexes.find(fromstop);
    if(got == stop_indexes.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    got = stop_indexes.find(tostop);
    if(got == stop_indexes.end()){
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }

    // Suoritetaan Dijkstran algoritmi.

    return dijkstra(fromstop, tostop);
}

bool Datastructures::add_trip(RouteID routeid, std::vector<Time> const& stop_times)
{
    // Tarkistetaan onko matka olemassa.

    std::unordered_map<RouteID,std::vector<StopID>>::const_iterator got = routes.find(routeid);
    if(got == routes.end()){
        return false;
    }

    trip newTrip;
    newTrip.routetrip_id = routeid;
    newTrip.stop_times = stop_times;
    trips.push_back(newTrip);
    ++trips_size;

    return true;
}

std::vector<std::pair<Time, Duration>> Datastructures::route_times_from(RouteID routeid, StopID stopid)
{
    int stop_location = 0;
    bool stop_found = false;
    std::vector<std::pair<Time, Duration>> results;

    // Tarkistetaan onko reittiä ja pysäkkiä olemassa.

    std::unordered_map<RouteID,std::vector<StopID>>::const_iterator got = routes.find(routeid);
    if(got == routes.end()){
        return {{NO_TIME, NO_DURATION}};
    }

    std::unordered_map<StopID,int>::const_iterator got2 = stop_indexes.find(stopid);
    if(got2 == stop_indexes.end()){
        return {{NO_TIME, NO_DURATION}};
    }

    // Etsitään pysäkki reitiltä.

    for(size_t i = 0; i < routes[routeid].size(); ++i){
        if(routes[routeid].at(i) == stopid){
            stop_location = i;
            stop_found = true;
            break;
        }
    }
    if(!stop_found) { return {{NO_TIME, NO_DURATION}}; }

    // Etsitään reitiltä lähtevät bussit ja niiden aikatauluja.

    for(int j = 0; j < trips_size; ++j){
        if(trips[j].routetrip_id == routeid){
            if(stop_location < trips_size){
                Duration to_next_stop = trips[j].stop_times[stop_location+1] - trips[j].stop_times[stop_location];
                results.push_back(std::make_pair(trips[j].stop_times[stop_location],to_next_stop));
            }
            else {
                results.push_back(std::make_pair(trips[j].stop_times[stop_location],0));
            }
        }
    }

    return results;
}

// En saanut tätä funktiota toimimaan.

std::vector<std::tuple<StopID, RouteID, Time> > Datastructures::journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime)
{
    return {{NO_STOP, NO_ROUTE, NO_TIME}};
}

// En saanut tätä funktiota toimimaan.

void Datastructures::add_walking_connections()
{
    // Replace this comment and the line below with your implementation
}


// Omat funktiot

// Tätä funktiota käytetään pitämään yllä id-listoja. Tätä kutsutaan, jos stoppeja
// sortataan ja näitä id-listoja voidaan sitten palauttaa kun niitä kysytään.
// Hyötynä näissä listoissa on se, että jos lisäyksiä tai poistoja ei ole tehty,
// ei tarvitse sortata uudelleen, kun voidaan vain palauttaa id-listat.

void Datastructures::update_IDs(std::string mode){
    if(mode == "name"){
       stopIDs_name.clear();
       for(int i = 0; i < stops_size; ++i){
            stopIDs_name.push_back(stops_temp[i].id);
       }
    }
    else {
        stopIDs_coord.clear();
        for(int i = 0; i < stops_size; ++i){
             stopIDs_coord.push_back(stops_temp[i].id);
        }
    }
}

// Mergesortin merge-operaatio, jossa yhdistetään kaksi osaa.

void Datastructures::merge(int p, int q, int r, std::string mode){
    int size1 = q-p+1;
    int size2 = r-q;
    std::vector<stop> L(size1);
    std::vector<stop> R(size2);

    for(int i = 0; i < size1; i++)
    {
        L[i] = stops_temp[p+i];
    }
    for(int j = 0; j < size2; j++)
    {
        R[j] = stops_temp[q+j+1];
    }

    int i=0,j=0;
    int k;

    for(k = p; k <= r && i < size1 && j < size2; k++)
     {
         if(comparison(L[i], R[j], mode))   // Tehdään vertailu.
         {
             stops_temp[k] = L[i];
             i++;
         }
         else
         {
             stops_temp[k] = R[j];
             j++;
         }
     }
    for(i = i; i < size1; ++i)
    {
        stops_temp[k] = L[i];
        k++;
    }

    for(j = j; j < size2; j++)
    {
        stops_temp[k] = R[j];
        k++;
    }
}

// Mergesortin operaation, jossa taulukko jaetaan osiin.

void Datastructures::mergesort(int p, int r, std::string mode){
    if (p < r)
        {
            int q = (p+r)/2;

            // Käsittele molemmat osat.
            mergesort(p, q, mode);
            mergesort(q+1, r, mode);
            merge(p, q, r, mode);
        }
}

// Tällä funktiolla tehdään vertailu kahden stopin välillä.
// Vertailu voi koskea joko nimeä tai koordinaatteja.

bool Datastructures::comparison(stop stop1, stop stop2, std::string mode){
    if(mode == "name"){
        if(stop1.name <= stop2.name){
            return true;
        }
        else{
            return false;
        }
    }
    else {
        double value1 = sqrt(pow(stop1.coordinates.x,2.0)+pow(stop1.coordinates.y,2.0));
        double value2 = sqrt(pow(stop2.coordinates.x,2.0)+pow(stop2.coordinates.y,2.0));
        if(value1 <= value2){
            return true;
        }
        else {
            return false;
        }
    }
}

// Tämä on rekursiivinen funktio, jolla mennään läpi stopin alueet menemällä aina
// ylemmälle tasolle ja laittamalla alueet talteen. Tätä käytetään funktiossa
// stop_regions, kun haluttiin tietää kaikki stopin ylemmät alueet.

void Datastructures::region_path(int index){
    stop_regions_results.push_back(regions[index].id);
    if(regions[index].has_parent){                 // Jos stoppi kuuluu johonkin alueeseen.
        region_path(regions[index].parent_index);
    }
}

// Tämä funktion pitää yllä pienimmän ja isoimman koordinaattien omaava stoppia.
// Se saa syötteenä stopin, jonka koordinaatteita vertaillaan vertailukohtiin.
// Jos sen koordinaatit ovat joko pienimmät tai isoimmat, päivitetään tallessa
// olevia muuttujia.

void Datastructures::set_min_max_stop(stop newstop){

    if(stops_size == 1){               // Jos on ensimmäinen stoppi. Se on suurin ja pienin (koordinaatit).
        min_coord_stop = newstop.coordinates;
        min_stop_stop = newstop.id;
        max_coord_stop = newstop.coordinates;
        max_stop_stop = newstop.id;
    }
    else{
        double value1 = sqrt(pow(newstop.coordinates.x,2.0)+pow(newstop.coordinates.y,2.0));
        double value2 = sqrt(pow(min_coord_stop.x,2.0)+pow(min_coord_stop.y,2.0));
        if(value1 < value2){
            min_coord_stop = newstop.coordinates;
            min_stop_stop = newstop.id;
        }
        value2 = sqrt(pow(max_coord_stop.x,2.0)+pow(max_coord_stop.y,2.0));
        if(value1 > value2){
            max_coord_stop = newstop.coordinates;
            max_stop_stop = newstop.id;
        }
    }
}

// Käytetään, kun etsitään kaikki alueen ja ali-alueiden pysäkin. Tässä mennään
// rekursiivisesti jokainen läpi ja talletetaan ne muistiin.

void Datastructures::find_region_stops(int region_index){
    for(size_t j = 0; j < regions[region_index].region_stops_index.size(); ++j){
            region_stops_results.push_back(regions[region_index].region_stops_index[j]);
    }
    if(regions[region_index].subregions_index.size() != 0){
        for(size_t i = 0; i < regions[region_index].subregions_index.size(); ++i){
            find_region_stops(regions[region_index].subregions_index[i]);
        }
    }
}

// Tätä funktiota käytetään, kun halutaan tietää kuuluuko jokin alue jo toisen ali-alueeseen.
// Siinä käydään toisen ali-alueet läpi ja vertaillaan sen id:t jokaiseen niistä ja lopetetaan etsintä, kun löytyy.
// Kuten edellisessäkin, tehokkuus olisi O(n).

bool Datastructures::find_subregions(int region_index, RegionID id){
    if(regions[region_index].id == id){
        return false;
    }
    if(regions[region_index].subregions_index.size() != 0){
        for(size_t i = 0; i < regions[region_index].subregions_index.size(); ++i){
            if(!find_subregions(regions[region_index].subregions_index[i],id)){
                return false;
            }
        }
    }
    return true;
}

int Datastructures::distance_between_stops(int stop1, int stop2){
    return floor(sqrt(pow(stops[stop1].coordinates.x-stops[stop2].coordinates.x,2.0)+pow(stops[stop1].coordinates.y-stops[stop2].coordinates.y,2.0)));
}

// BFS-algorimit toteutus

std::vector<std::tuple<StopID, int, RouteID, Distance>> Datastructures::bfs(StopID src, StopID target) {

    std::list<StopID> q;
    std::unordered_map<StopID, bool> visited;
    std::unordered_map<StopID,int> dist;
    std::vector<std::tuple<StopID, int, RouteID, Distance>> prev;  // Current, Prev, Route, Dist
    
    q.push_back(src);
    visited[src] = true;
    dist[src] = 0;
    prev.push_back(std::make_tuple(src,-1,NO_ROUTE,0));

    // Käydään jokainen pysäkki ja sitten niiden viereiset pysäkit lävitse.

    while(!q.empty()){
        StopID node = q.front();
        q.pop_back();

        for(std::pair<RouteID, std::pair<int,int>> neighbor : stops[stop_indexes[node]].next_stop)
        {
            if(!visited[stops[neighbor.second.first].id])
            {
                dist[stops[neighbor.second.first].id] = dist.at(node) + neighbor.second.second;
                q.push_back(stops[neighbor.second.first].id);
                prev.push_back(std::make_tuple(stops[neighbor.second.first].id, stop_indexes[node], neighbor.first, dist[stops[neighbor.second.first].id]));
                visited[stops[neighbor.second.first].id] = true;
                if(stops[neighbor.second.first].id == target){     // Kohde-pysäkki löydetty.
                    return prev;
                }
            }
        }
    }
    return prev;
}

// Funktio, jossa otetaan haluttu reitti talteen BFS-algoritmilta mennen kohteesta lähtöpaikkaan.

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::route_tracker(std::vector<std::tuple<StopID, int, RouteID, Distance>> route_map){

    std::vector<std::tuple<StopID, RouteID, Distance>> result;
    int index = route_map.size()-1;
    RouteID route_taken;
    StopID test;

    result.push_back(std::make_tuple(std::get<0>(route_map[index]),NO_ROUTE,std::get<3>(route_map[index])));

    while(std::get<1>(route_map[index]) != -1){
        route_taken = std::get<2>(route_map[index]);
        test = std::get<0>(route_map[index]);
        index = std::get<1>(route_map[index]);
        result.push_back(std::make_tuple(std::get<0>(route_map[index]),route_taken,std::get<3>(route_map[index])));
    }
    std::reverse(result.begin(),result.end());

    return result;
}

// Apufunktio Dijkstran algoritmille.

StopID Datastructures::minDistance(std::unordered_map<StopID,int> dist, std::unordered_map<StopID,bool> sptSet)
{
    // Initialize min value
    int min = std::numeric_limits<int>::max();
    StopID min_index;

    for (int v = 0; v < stops_size; v++){
        if (sptSet.at(stops[v].id) == false && dist.at(stops[v].id) <= min){
            min = dist.at(stops[v].id);
            min_index = stops[v].id;
        }
    }

    return min_index;
}

// Dijkstran algoritmin toteutus.

std::vector<std::tuple<StopID, RouteID,Distance>> Datastructures::dijkstra(StopID src, StopID target){

    std::unordered_map<StopID,StopID> parent;
    std::unordered_map<StopID,RouteID> routes;
    std::unordered_map<StopID,bool> visited;
    std::unordered_map<StopID,int> dist;

    for(size_t i = 0; i < stops.size(); i++)
    {
        visited[stops[i].id] = false;
        dist[stops[i].id] = std::numeric_limits<int>::max();
    }

    dist[src] = 0;
    parent[src] = NO_STOP;
    StopID u, v;

    for(int count = 0; count < stops_size - 1; ++count){

        u = minDistance(dist, visited);
        visited[u] = true;

        for(std::pair<RouteID, std::pair<int,int>> neighbor : stops[stop_indexes[u]].next_stop)
        {
            v = stops[neighbor.second.first].id;

            if(!visited.at(v) && dist.at(u) + neighbor.second.second < dist.at(v)){
                dist[v] = dist.at(u) + neighbor.second.second;
                parent[v] = u;
                routes[u] = neighbor.first;
            }
        }
    }
    routes[v] = NO_ROUTE;
    dijkstra_results.clear();
    return create_path(parent,dist,src,target,routes);
}

// Käytetään Dijkstran algoritmin reitin talletukseen.

std::vector<std::tuple<StopID, RouteID,Distance>> Datastructures::create_path(std::unordered_map<StopID,StopID> parent, std::unordered_map<StopID,int> dist, StopID src, StopID target,std::unordered_map<StopID,RouteID> routes){

    if(target == src) {
        dijkstra_results.push_back(std::make_tuple(target,routes.at(target),dist.at(target)));
        return dijkstra_results; }

    create_path(parent,dist,src,parent.at(target),routes);

    dijkstra_results.push_back(std::make_tuple(target,routes.at(target),dist.at(target)));

    return dijkstra_results;
}
