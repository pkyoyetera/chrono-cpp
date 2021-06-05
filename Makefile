

debug:
	bazel build -c dbg ...

release:
	bazel build -c opt :time

clean:
	bazek clean

test:
	bazel test ...