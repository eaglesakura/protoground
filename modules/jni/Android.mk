LOCAL_PATH := $(call my-dir)
# ndk-build -B NDK_DEBUG=1 -j8

#######################################################################
##                           Prebuilt Modules                        ##
#######################################################################
include $(CLEAR_VARS)

LOCAL_MODULE := prebuilt-protoground
LOCAL_CPP_FLAGS += -std=gnu++11

############################################################
##                     Arch Options                       ##
############################################################
LOCAL_CPPFLAGS += -pthread
LOCAL_CPPFLAGS +=-ffast-math
LOCAL_CFLAGS   +=-ffast-math
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a-hard)
	LOCAL_ARM_MODE := arm
	LOCAL_ARM_NEON := true
	LOCAL_CPPFLAGS += -mfpu=neon
	LOCAL_CFLAGS += -mfpu=neon
endif

ifeq ($(TARGET_ARCH_ABI), x86)
	LOCAL_ARM_MODE := arm
	LOCAL_ARM_NEON := true
	LOCAL_CPPFLAGS += -mfpmath=sse -msse2 -msse3
	LOCAL_CFLAGS += -mfpmath=sse -msse2 -msse3
endif

ifeq ($(TARGET_ARCH_ABI), x86_64)
	LOCAL_CPPFLAGS += -mfpmath=sse -msse2 -msse3
	LOCAL_CFLAGS += -mfpmath=sse -msse2 -msse3
endif

############################################################
##                       sqlite3                          ##
############################################################
LOCAL_CFLAGS += -DSQLITE_DEFAULT_JOURNAL_SIZE_LIMIT=1048576
LOCAL_CFLAGS += -DSQLITE_THREADSAFE=1
LOCAL_CFLAGS += -DSQLITE_ENABLE_MEMORY_MANAGEMENT=1
LOCAL_CFLAGS += -DSQLITE_DEFAULT_AUTOVACUUM=1
LOCAL_CFLAGS += -DSQLITE_TEMP_STORE=3
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3
LOCAL_CFLAGS += -DSQLITE_ENABLE_FTS3_BACKWARDS
LOCAL_CFLAGS += -DSQLITE_ENABLE_POISON

# LOCAL_SRC_FILES += ../sqlite3/shell.c
LOCAL_SRC_FILES += ../sqlite3/sqlite3.c

############################################################
##                       freetype                         ##
############################################################
LOCAL_CFLAGS := -DANDROID_NDK
LOCAL_CFLAGS := -DFT2_BUILD_LIBRARY=1

LOCAL_C_INCLUDES += ../freetype/devel
LOCAL_C_INCLUDES += ../freetype/include
LOCAL_C_INCLUDES += ../freetype/src

LOCAL_SRC_FILES += ../freetype/src/autofit/autofit.c
LOCAL_SRC_FILES += ../freetype/src/base/basepic.c
LOCAL_SRC_FILES += ../freetype/src/base/ftapi.c
LOCAL_SRC_FILES += ../freetype/src/base/ftbase.c
LOCAL_SRC_FILES += ../freetype/src/base/ftbbox.c
LOCAL_SRC_FILES += ../freetype/src/base/ftbitmap.c
LOCAL_SRC_FILES += ../freetype/src/base/ftglyph.c
LOCAL_SRC_FILES += ../freetype/src/base/ftinit.c
LOCAL_SRC_FILES += ../freetype/src/base/ftpic.c
LOCAL_SRC_FILES += ../freetype/src/base/ftstroke.c
LOCAL_SRC_FILES += ../freetype/src/base/ftsynth.c
LOCAL_SRC_FILES += ../freetype/src/base/ftsystem.c
LOCAL_SRC_FILES += ../freetype/src/cff/cff.c
LOCAL_SRC_FILES += ../freetype/src/pshinter/pshinter.c
LOCAL_SRC_FILES += ../freetype/src/psnames/psnames.c
LOCAL_SRC_FILES += ../freetype/src/raster/raster.c
LOCAL_SRC_FILES += ../freetype/src/sfnt/sfnt.c
LOCAL_SRC_FILES += ../freetype/src/smooth/smooth.c
LOCAL_SRC_FILES += ../freetype/src/truetype/truetype.c
LOCAL_SRC_FILES += ../freetype/src/psaux/psaux.c
LOCAL_SRC_FILES += ../freetype/src/type1/type1.c
LOCAL_SRC_FILES += ../freetype/src/type42/type42.c
LOCAL_SRC_FILES += ../freetype/src/bdf/bdf.c
LOCAL_SRC_FILES += ../freetype/src/cid/type1cid.c
LOCAL_SRC_FILES += ../freetype/src/pfr/pfr.c
LOCAL_SRC_FILES += ../freetype/src/pcf/pcf.c
LOCAL_SRC_FILES += ../freetype/src/winfonts/winfnt.c

LOCAL_SRC_FILES += ../freetype/src/base/ftdbgmem.c
LOCAL_SRC_FILES += ../freetype/src/base/ftdebug.c

############################################################
##                       libpng                           ##
############################################################

LOCAL_SRC_FILES += ../libpng/jni/png.c
LOCAL_SRC_FILES += ../libpng/jni/pngerror.c
LOCAL_SRC_FILES += ../libpng/jni/pngget.c
LOCAL_SRC_FILES += ../libpng/jni/pngmem.c
LOCAL_SRC_FILES += ../libpng/jni/pngpread.c
LOCAL_SRC_FILES += ../libpng/jni/pngread.c
LOCAL_SRC_FILES += ../libpng/jni/pngrio.c
LOCAL_SRC_FILES += ../libpng/jni/pngrtran.c
LOCAL_SRC_FILES += ../libpng/jni/pngrutil.c
LOCAL_SRC_FILES += ../libpng/jni/pngset.c
LOCAL_SRC_FILES += ../libpng/jni/pngtrans.c
LOCAL_SRC_FILES += ../libpng/jni/pngwio.c
LOCAL_SRC_FILES += ../libpng/jni/pngwrite.c
LOCAL_SRC_FILES += ../libpng/jni/pngwtran.c
LOCAL_SRC_FILES += ../libpng/jni/pngwutil.c
LOCAL_SRC_FILES += ../libpng/jni/arm/arm_init.c
LOCAL_SRC_FILES += ../libpng/jni/arm/filter_neon_intrinsics.c
LOCAL_SRC_FILES += ../libpng/jni/arm/filter_neon.S
# LOCAL_SRC_FILES += ../libpng/jni/pngtest.c

############################################################
##                       bullet3                          ##
############################################################
LOCAL_C_INCLUDES += ../bullet3/src/

LOCAL_SRC_FILES += ../bullet3/src/Bullet3Common/b3AlignedAllocator.cpp
LOCAL_SRC_FILES += ../bullet3/src/Bullet3Common/b3Logging.cpp
LOCAL_SRC_FILES += ../bullet3/src/Bullet3Common/b3Vector3.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btDbvt.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btGhostObject.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/btUnionFind.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btBox2dShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btBoxShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btCollisionShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btCompoundShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConcaveShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConeShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btCylinderShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btEmptyShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btShapeHull.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btSphereShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btContactProcessing.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btGImpactBvh.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btGImpactShape.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/gim_box_set.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/gim_contact.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/gim_memory.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/Gimpact/gim_tri_collision.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Character/btKinematicCharacterController.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Dynamics/btRigidBody.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBody.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletDynamics/Vehicle/btWheelInfo.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btDefaultSoftBodySolver.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftBody.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftBodyHelpers.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftRigidDynamicsWorld.cpp
LOCAL_SRC_FILES += ../bullet3/src/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btAlignedAllocator.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btConvexHull.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btConvexHullComputer.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btGeometryUtil.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btPolarDecomposition.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btQuickprof.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btSerializer.cpp
LOCAL_SRC_FILES += ../bullet3/src/LinearMath/btVector3.cpp


include $(BUILD_STATIC_LIBRARY)
