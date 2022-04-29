/*
 * namespaces.h
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#ifndef INCLUDE_BETTERCPP_NAMESPACES_H_
#define INCLUDE_BETTERCPP_NAMESPACES_H_

#ifdef __cplusplus

#define NSP_BETTERCPP bettercpp

#define NSP_BETTERCPP_BEGIN namespace NSP_BETTERCPP {
#define NSP_BETTERCPP_END }

#define NSP_TESTS_BEGIN NSP_BETTERCPP_BEGIN namespace Tests {
#define NSP_TESTS_END NSP_BETTERCPP_END }

#endif // __cplusplus


#endif /* INCLUDE_BETTERCPP_NAMESPACES_H_ */
