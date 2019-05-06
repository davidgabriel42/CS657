#include <tiledb/tiledb>
#include <ctime>
#include <iostream>

/*
changed to input operands for all funcs
//global benchmark vars
int base = 1;
int n =  20000000;
int val = 1;
int nrows = 16;
int ncols =  125000000;
*/
// Name of array.
std::string array_name("quickstart_sparse_array");

void create_array(int base, int n, int val, int nrows, int ncols) {
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

void write_array(int base, int n, int val, int nrows, int ncols) {
  tiledb::Context ctx;

  // Prepare some data for the array

std::vector<int32_t> data;
data.assign(n,val);

std::vector<int> coords;

int k = base;
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
int base = 1;
int n =  20000000;
int val = 1;
int nrows = 16;
int ncols =  125000000;

time_t tstart, t1, t2, t3, t4, t5 , tend;

std::cout << "nrows= "<< std::to_string(nrows) << std::endl;
std::cout << "ncols= "<< std::to_string(ncols) << std::endl;
std::cout << "ndata= "<< std::to_string(n) << std::endl;

std::cout << "Each write block represents 10% of data, written contiguously."<< std::endl;

tstart = time(0);


create_array(base,n,val,nrows,ncols);
t1 = time(0);
std::cout << "Array initialization took  "<< difftime(t1, tstart) <<" second(s)."<< std::endl;

write_array(base,n,val,nrows,ncols);
t2 = time(0);
std::cout << "First write block took  "<< difftime(t2, t1) <<" second(s)."<< std::endl;

base += n;
write_array(base,n,val,nrows,ncols);
t3 = time(0);
std::cout << "Second write block took "<< difftime(t3, t2) <<" second(s)."<< std::endl;


base += n;
write_array(base,n,val,nrows,ncols);
t4 = time(0);
std::cout << "Third write block took "<< difftime(t4, t3) <<" second(s)."<< std::endl;

base += n;
write_array(base,n,val,nrows,ncols);
t5 = time(0);

std::cout << "Fourth write block took "<< difftime(t5, t4) <<" second(s)."<< std::endl;

base += n;
write_array(base,n,val,nrows,ncols);
tend = time(0);

std::cout << "Fifth write block took "<< difftime(t5, t4) <<" second(s)."<< std::endl;

std::cout << "Total time was  "<< difftime(tend, tstart) <<" second(s)."<< std::endl;

}
