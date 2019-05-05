#include <tiledb/tiledb>
#include <ctime>
#include <iostream>

//global benchmark vars
int n =  100000000;
int val = 1;
int nrows = 16;
int ncols =  125000000;

// Name of array.
std::string array_name("quickstart_sparse_array");

void create_array() {
  // Create a TileDB context.
  tiledb::Context ctx;

  // The array will be 4x4 with dimensions "rows" and "cols", with domain [1,4].
  tiledb::Domain domain(ctx);
  domain.add_dimension(tiledb::Dimension::create<int>(ctx, "rows", {{1,ncols}},  1000))
        .add_dimension(tiledb::Dimension::create<int>(ctx, "cols", {{1, nrows}},  4));

  // The array will be dense.
  tiledb::ArraySchema schema(ctx, TILEDB_SPARSE);
  schema.set_domain(domain)
        .set_order({{TILEDB_ROW_MAJOR, TILEDB_ROW_MAJOR}});

  // Add a single attribute "a" so each (i,j) cell can store an integer.
  schema.add_attribute(tiledb::Attribute::create<int>(ctx, "a"));

  // Create the (empty) array on disk.
  tiledb::Array::create(array_name, schema);
}

void write_array() {
  tiledb::Context ctx;

  // Prepare some data for the array

std::vector<int32_t> data;
data.assign(n,val);

std::vector<int> coords;

int k = 1;
for(int i = 1; i <= n/nrows; i++)
{

	for (int j = 1; j<= nrows; j++)
	{
	coords.push_back(k);
	coords.push_back(j);
	}
k++;
}
//std::cout<<coords;

  // Open the array for writing and create the query.
  tiledb::Array array(ctx, array_name, TILEDB_WRITE);
  tiledb::Query query(ctx, array);
  query.set_layout(TILEDB_UNORDERED)
       .set_buffer("a", data)
       .set_coordinates(coords);
  // Perform the write and close the array.
  query.submit();
  array.close();
}

int main(){

time_t tstart, tend; 
 tstart = time(0);
create_array();
write_array();
tend = time(0); 

std::cout << "nrows= "<< std::to_string(nrows) << std::endl;
std::cout << "ncols= "<< std::to_string(ncols) << std::endl;
std::cout << "ndata= "<< std::to_string(n) << std::endl;

std::cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< std::endl;
}
