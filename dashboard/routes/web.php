<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});

Route::resource('resources', 'ResourcesController');
Route::delete('/resources', 'ResourcesController@destroy');
Route::put('/resources', 'ResourcesController@update');
Auth::routes(['register'=>false]);

Route::get('/home', 'HomeController@index');
