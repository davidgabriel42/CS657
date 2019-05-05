#include <tiledb/tiledb>

// Name of array.
std::string array_name("quickstart_dense_array");

void create_array() {
  // Create a TileDB context.
  tiledb::Context ctx;

  // The array will be 4x4 with dimensions "rows" and "cols", with domain [1,4].
  tiledb::Domain domain(ctx);
  domain.add_dimension(tiledb::Dimension::create<int>(ctx, "rows", {{1, 4}}, 4))
        .add_dimension(tiledb::Dimension::create<int>(ctx, "cols", {{1, 4}}, 4));

  // The array will be dense.
  tiledb::ArraySchema schema(ctx, TILEDB_DENSE);
  schema.set_domain(domain)
        .set_order({{TILEDB_ROW_MAJOR, TILEDB_ROW_MAJOR}});

  // Add a single attribute "a" so each (i,j) cell can store an integer.
  schema.add_attribute(tiledb::Attribute::create<int>(ctx, "a"));

  // Create the (empty) array on disk.
  tiledb::Array::create(array_name, schema);
}

int main(){
create_array();
}
