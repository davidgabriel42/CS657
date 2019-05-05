#include <tiledb/tiledb>
#include <ctime>

// Name of array.
std::string array_name("quickstart_sparse_array");

void create_array() {
  // Create a TileDB context.
  tiledb::Context ctx;

  // The array will be 4x4 with dimensions "rows" and "cols", with domain [1,4].
  tiledb::Domain domain(ctx);
  domain.add_dimension(tiledb::Dimension::create<int>(ctx, "rows", {{1, 125000000}},  1000))
        .add_dimension(tiledb::Dimension::create<int>(ctx, "cols", {{1, 16}},  4));

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
 int n =  2000000000;
 int val = 1;
 std::vector<int32_t> data;
 data.assign(n,val);

  // Open the array for writing and create the query.
  tiledb::Array array(ctx, array_name, TILEDB_WRITE);
  tiledb::Query query(ctx, array);
  query.set_layout(TILEDB_UNORDERED)
       .set_buffer("a", data);

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
std::cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< std::endl;
}
