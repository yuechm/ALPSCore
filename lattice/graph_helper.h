/*****************************************************************************
*
* ALPS Project: Algorithms and Libraries for Physics Simulations
*
* ALPS Libraries
*
* Copyright (C) 2000-2004 by Matthias Troyer <troyer@itp.phys.ethz.ch>
*
* This software is part of the ALPS libraries, published under the ALPS
* Library License; you can use, redistribute it and/or modify it under
* the terms of the license, either version 1 or (at your option) any later
* version.
* 
* You should have received a copy of the ALPS Library License along with
* the ALPS Libraries; see the file LICENSE.txt. If not, the license is also
* available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

/* $Id$ */

#ifndef ALPS_SCHEDULER_LATTICEHELPER_H
#define ALPS_SCHEDULER_LATTICEHELPER_H


#include <alps/lattice/latticelibrary.h>
#include <alps/lattice/disorder.h>
#include <boost/vector_property_map.hpp>

namespace alps {

template <class G=coordinate_graph_type>
class graph_helper : public LatticeLibrary
{
public:
  typedef G graph_type;
  typedef typename graph_traits<graph_type>::vertex_iterator vertex_iterator;
  typedef typename graph_traits<graph_type>::edge_iterator edge_iterator;
  typedef typename graph_traits<graph_type>::out_edge_iterator out_edge_iterator;
  typedef typename graph_traits<graph_type>::in_edge_iterator in_edge_iterator;
  typedef typename graph_traits<graph_type>::edge_descriptor edge_descriptor;
  typedef typename graph_traits<graph_type>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<graph_type>::vertices_size_type vertices_size_type;
  typedef typename graph_traits<graph_type>::edges_size_type edges_size_type;
  typedef typename graph_traits<graph_type>::degree_size_type degree_size_type;
  typedef typename graph_traits<graph_type>::adjacency_iterator adjacency_iterator;
  
  typedef typename graph_traits<graph_type>::site_iterator site_iterator;
  typedef typename graph_traits<graph_type>::bond_iterator bond_iterator;
  typedef typename graph_traits<graph_type>::neighbor_bond_iterator neighbor_bond_iterator;
  typedef typename graph_traits<graph_type>::bond_descriptor bond_descriptor;
  typedef typename graph_traits<graph_type>::site_descriptor site_descriptor;
  typedef typename graph_traits<graph_type>::sites_size_type sites_size_type;
  typedef typename graph_traits<graph_type>::bonds_size_type bonds_size_type;
  typedef typename graph_traits<graph_type>::neighbors_size_type neighbors_size_type;
  typedef typename graph_traits<graph_type>::neighbor_iterator neighbor_iterator;

 graph_helper(std::istream& in, const Parameters& p)
   : LatticeLibrary(in), 
	 to_delete_(false), 
     g_(make_graph(p)),
     is_bipartite_(set_parity(graph())),
     parity_map_(get_or_default(parity_t(),const_graph(),0.)),
     edge_type_map_(get_or_default(edge_type_t(),const_graph(),0.)),
     vertex_type_map_(get_or_default(vertex_type_t(),const_graph(),0.)),
     coordinate_map_(get_or_default(coordinate_t(),const_graph(),0)),
	 disordered_vertex_type_map_(),
	 disordered_edge_type_map_(get_or_default(edge_index_t(),const_graph(),0))
  {
	d_.disorder_vertices(graph(),disordered_vertex_type_map_);
	d_.disorder_edges(graph(),disordered_edge_type_map_);
  }
  
  
  graph_helper(const alps::Parameters& p)
   : LatticeLibrary(p), 
	 to_delete_(false), 
     g_(make_graph(p)),
     is_bipartite_(set_parity(graph())),
     parity_map_(get_or_default(parity_t(),const_graph(),0.)),
     edge_type_map_(get_or_default(edge_type_t(),const_graph(),0)),
     vertex_type_map_(get_or_default(vertex_type_t(),const_graph(),0)),
     coordinate_map_(get_or_default(coordinate_t(),const_graph(),std::vector<double>())),
	 disordered_vertex_type_map_(),
	 disordered_edge_type_map_(get_or_default(edge_index_t(),const_graph(),0))
  {
	d_.disorder_vertices(graph(),disordered_vertex_type_map_);
	d_.disorder_edges(graph(),disordered_edge_type_map_);
  }

  ~graph_helper() { if (to_delete_) delete g_;}
  graph_type& graph() { return *g_;}
  const graph_type& graph() const { return *g_;}
  
  sites_size_type num_sites() const { return alps::num_sites(graph());}
  bonds_size_type num_bonds() const { return alps::num_bonds(graph());}
  std::pair<site_iterator,site_iterator> sites() const { return alps::sites(graph());}
  std::pair<bond_iterator,bond_iterator> bonds() const { return alps::bonds(graph());}
  bond_descriptor bond(bonds_size_type i) const { return *(bonds().first+i);}
  neighbors_size_type num_neighbors (const site_descriptor& v) const { return alps::num_neighbors(v,graph());}
  std::pair<neighbor_bond_iterator,neighbor_bond_iterator> neighbor_bonds (const site_descriptor& v) const 
    { return alps::neighbor_bonds(v,graph());}
  std::pair<neighbor_iterator,neighbor_iterator> neighbors (const site_descriptor& v) const 
    { return alps::neighbors(v,graph());}
  site_descriptor neighbor (const site_descriptor& v, neighbors_size_type i) const { return alps::neighbor(v,i,graph());} 
  site_descriptor site(sites_size_type i) const { return alps::site(i,graph());}
  site_descriptor source(const bond_descriptor& b) const { return alps::source_impl(b,graph());}  
  site_descriptor target(const bond_descriptor& b) const { return alps::target_impl(b,graph());}  
  
  vertices_size_type num_vertices() const { return num_vertices(graph());}
  edges_size_type num_edges() const { return num_edges(graph());}
  std::pair<vertex_iterator,vertex_iterator> vertices() const { return vertices(graph());}
  std::pair<edge_iterator,edge_iterator> edges() const { return edges(graph());}
  degree_size_type out_degree (const vertex_descriptor& v) const { return out_degree(v,graph());}
  degree_size_type in_degree (const vertex_descriptor& v) const { return in_degree(v,graph());}
  degree_size_type degree (const vertex_descriptor& v) const { return degree(v,graph());}
  out_edge_iterator out_edges (const vertex_descriptor& v) const { return out_edges(v,graph());}
  in_edge_iterator in_edges (const vertex_descriptor& v) const { return in_edges(v,graph());}
  std::pair<adjacency_iterator,adjacency_iterator> adjacent_vertices (const site_descriptor& v) const 
  { return adjacent_vertices(v,graph());}
  vertex_descriptor vertex(vertices_size_type i) const { return vertex(i,graph());}
  double parity(const site_descriptor& v) const { return parity_map_[v]==0 ? 1. :  parity_map_[v]==1 ? -1. : 0.;}
  bool is_bipartite() const { return is_bipartite_;}
  unsigned int bond_type(const bond_descriptor& b) const { return edge_type_map_[b];}
  unsigned int edge_type(const edge_descriptor& e) const { return edge_type_map_[e];}
  unsigned int site_type(const site_descriptor& s) const { return vertex_type_map_[s];}
  unsigned int vertex_type(const edge_descriptor& v) const { return vertex_type_map_[v];}
  unsigned int disordered_edge_type(const edge_descriptor& e) const 
  { return d_.disordered_edges() ? disordered_edge_type_map_[e] : edge_type_map_[e];}
  unsigned int disordered_bond_type(const bond_descriptor& b) const { return disordered_edge_type(b);}
  unsigned int disordered_vertex_type(const vertex_descriptor& v) const 
  { return d_.disordered_vertices() ? disordered_vertex_type_map_[v] : vertex_type_map_[v];}
  unsigned int disordered_site_type(const site_descriptor& s) const { return disordered_vertex_type(s);}
  bool disordered() const { return d_.disordered();}
  bool disordered_sites() const { return d_.disordered_sites();}
  bool disordered_bonds() const { return d_.disordered_bonds();}
  bool disordered_vertices() const { return d_.disordered_vertices();}
  bool disordered_edges() const { return d_.disordered_edges();}
  const std::vector<double>& coordinate(const site_descriptor& s) const { return coordinate_map_[s];}
private:
  typedef lattice_graph<hypercubic_lattice<coordinate_lattice<simple_lattice<GraphUnitCell> > >,graph_type> lattice_type;
  graph_type* make_graph(const Parameters& p);
  const graph_type& const_graph() const { return *g_;}

  lattice_type l_;
  bool to_delete_;
  DisorderDescriptor d_;
  graph_type* g_;
  bool is_bipartite_;
  typename property_map<parity_t,graph_type,double>::const_type parity_map_;
  typename property_map<edge_type_t,graph_type,unsigned int>::const_type edge_type_map_;
  typename property_map<edge_index_t,graph_type,unsigned int>::const_type edge_index_map_;
  typename property_map<vertex_type_t,graph_type,unsigned int>::const_type vertex_type_map_;
  typename property_map<coordinate_t,graph_type,std::vector<double> >::const_type coordinate_map_;
  boost::vector_property_map<unsigned int> disordered_vertex_type_map_;
  boost::vector_property_map<unsigned int,typename property_map<edge_index_t,graph_type,unsigned int>::const_type> disordered_edge_type_map_;
};


template <class G>
G* graph_helper<G>::make_graph(const Parameters& parms)
{
  std::string name;
  bool have_graph=false;
  bool have_lattice=false;
  graph_type* g;
  
  if (have_graph = parms.defined("GRAPH"))
    name = static_cast<std::string>(parms["GRAPH"]);
  if (have_lattice = parms.defined("LATTICE"))
    name = static_cast<std::string>(parms["LATTICE"]);
  if (have_lattice && have_graph)
    boost::throw_exception(std::runtime_error("both GRAPH and LATTICE were specified"));
  if (have_lattice && has_lattice(name)) {
    LatticeGraphDescriptor desc(lattice_descriptor(name));
    desc.set_parameters(parms);
    l_ = lattice_type(desc);
	d_ = desc.disorder();
    g = &(l_.graph());
    to_delete_=false;
  }
  else if ((have_lattice || have_graph) && has_graph(name)) {
    g = new graph_type();
    get_graph(*g,name);
    to_delete_=true;
  }
  else
    boost::throw_exception(std::runtime_error("could not find graph/lattice specified in parameters"));
  return g;
}

} // end namespace

#endif
