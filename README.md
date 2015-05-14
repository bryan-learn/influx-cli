Influx-cli
----------
CLI for InfluxDB used for the XSight project. Allows you to write data to,
and query, the database. For more information about the InfluxDB API see
www.influxdb.com

BUILD
-----
Building is simple:

    make

USAGE
-----
Synopsis:

    influx-cli [-q query_string] [-w json_string]

Examples:

    ./influx-cli -q "select * from my_series"
    ./influxctl -w '[{ "name": "test", "columns": ["value"], "points": [[8]] }]'

COPYRIGHT
---------
Copyright © 2015, Pittsburgh Supercomputing Center.  All Rights Reserved.
