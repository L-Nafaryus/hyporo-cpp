((nil . (
	(projectile-project-name . "hyporo")
	(projectile-project-compilation-dir . "")
	(projectile-enable-caching . t)
	(projectile-project-configure-cmd . "cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=DEBUG")
	(projectile-project-compilation-cmd . "cmake --build build ")
	(projectile-project-test-cmd . "ctest --test-dir build")
	(projectile-project-run-cmd . "cd build/source/creator && ./hyporo")
	(cmake-ide-build-dir . "build")
)))
